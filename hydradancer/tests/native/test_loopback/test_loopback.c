/********************************** (C) COPYRIGHT *******************************
* File Name          : hydradancer_usb_benchmark.c
* Author             : bvernoux
* Version            : V0.1.0
* Date               : 2023/08/06
* Description        :
* Copyright (c) 2023 Benjamin VERNOUX
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <cargs.h>

#include "libusb_portable.h"

struct libusb_device_handle* handle = NULL;
int usb_speed;

#define USB3_BULK_BURST_SIZE (4 * 1024) // This size corresponds to USB3 EPx Bulk with 4 burst(so 4x1024 bytes)
// Warning USB3 enpoint 2 bulk with 8 or 16 burst can be problematic on some PC so default is set to 4
#define USB2_BULK_SIZE 512

/* Big Buffer for EPx (Benchmark) */
#define TEST_INTEGRITY_NUM 20000
#define TEST_INTEGRITY_SIZE_NUM 5
#define TEST_NUM_SS 100 // Number of test for USB3 Super Speed(5Gbit/s)
#define TEST_NUM_HS 10 // Number of test for USB2 High Speed(480Mbit/s)
#define TEST_DATA_LEN 0x800000 // 8 MiB
//#define TEST_DATA_LEN 0x1000000 // 16 MiB fail on some PC with libusb (Can return libusb_bulk_transfer return error=-11 => NO MEM)

static __attribute__((aligned(128))) uint32_t mReadBigBuf[((TEST_DATA_LEN + 16) / sizeof(uint32_t))];
static __attribute__((aligned(128))) uint32_t mWriteBigBuf[((TEST_DATA_LEN + 16) / sizeof(uint32_t))];

FILE* pFile = NULL; /* File for logging */

void cleanup(void)
{
	if (pFile != NULL)
	{
		fclose(pFile);
		pFile = NULL;
	}
	if (handle != NULL)
	{
		usb_closedev(handle);
		handle = NULL;
	}
	usb_exit();
}

void error_exit(char* error_str)
{
	cleanup();
	if (error_str != NULL)
	{
		log_printf("%sTests end with failure(s)\n", error_str);
	}
	log_printf("Press Enter to exit\n");
	getchar();
	exit(-1);
}

#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal)
{
	if (signal == CTRL_C_EVENT)
	{
		log_printf("\nCtrl-C pressed\nExit\n");
		error_exit(NULL);
	}
	return TRUE;
}
#else
void consoleHandler(int s)
{
	if (s == SIGINT) // Ctrl-C
	{
		log_printf("\nCtrl-C pressed\nExit\n");
		error_exit(NULL);
	}
}
#endif

/**
 * This is the main configuration of all options available.
 */
static struct cag_option options[] = {
	{ .identifier = 'v',
	  .access_letters = "v",
	  .access_name = "verbose",
	  .value_name = "VALUE",
	  .description = "Verbose mode to have more detais on output" },

	{ .identifier = 'h',
	  .access_letters = "h",
	  .access_name = "help",
	  .description = "Shows the command help" }
};

//! Byte swap unsigned int
uint32_t swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

/* Return 0 if success other value are error code */
int USB_TestDataIntegrity(struct libusb_device_handle* handle, uint32_t* writebuf, uint32_t* readbuf, int test_num)
{
	int test_data_len;
	size_t mTotal;
	uint32_t test_data_start_val = 0;
	int ep_num;
	int mTestCount;
	double speed_mbytes_per_sec;
	float time_diff_s;

	struct timeval start_data;
	struct timeval curr_data;

	if (usb_speed == LIBUSB_SPEED_SUPER)
	{
		test_data_len = USB3_BULK_BURST_SIZE; /* USB3_BULK_BURST_SIZE Bytes for USB3 SS */
	}
	else
	{
		test_data_len = USB2_BULK_SIZE; /* 512 Bytes for USB2 HS */
	}

	for (ep_num = 1; ep_num < 8; ep_num++)
	{
		mTotal = 0;
		int bulk_ep_out = (ep_num | LIBUSB_ENDPOINT_OUT);
		int bulk_ep_in = (ep_num | LIBUSB_ENDPOINT_IN);

		log_printf("\nStart USB_TestDataIntegrity(libusb_bulk_transfer) EP%d Tests\n", ep_num);
		log_printf("libusb_bulk_transfer(WriteEP=0x%02X)/libusb_bulk_transfer(ReadEP=0x%02X)/memcmp: %d times %d Bytes ***\n", bulk_ep_out, bulk_ep_in, test_num, test_data_len);

		log_printf_dbg("Start Test\n");
		gettimeofday(&start_data, NULL);
		for (mTestCount = 0; mTestCount < test_num; ++mTestCount) // loop test
		{
			uint32_t* memWBuf = (uint32_t*)writebuf;
			for (uint32_t i = 0; i < (test_data_len / 4); i++)
			{
				memWBuf[i] = swap_uint32(i + test_data_start_val); // Swap from LE to BE
			}
			test_data_start_val += (test_data_len / 4);

			if (usb_write_EPx(handle, bulk_ep_out, (uint8_t*)writebuf, test_data_len) == 1)
			{
				mTotal += test_data_len;
			}
			else
			{
				log_printf("usb_write_EPx() error\n");
				log_printf("Test failure end with error\n");
				return -2;
			}

			memset(readbuf, 0x00, test_data_len);
			if (usb_read_EPx(handle, bulk_ep_in, (uint8_t*)readbuf, test_data_len) == 1)
			{
				mTotal += test_data_len;
			}
			else
			{
				log_printf("usb_read_EPx() error\n");
				log_printf("Test failure end with error\n");
				return -2;
			}

			if (memcmp(readbuf, writebuf, test_data_len) != 0)
			{
				log_printf("memcmp() failed mTestCount=%d\n", mTestCount);
				log_printf("Test failure end with error\n");
				return -2;
			}
		}
		gettimeofday(&curr_data, NULL);
		log_printf_dbg("End Test\n");
		time_diff_s = TimevalDiff(&curr_data, &start_data);
		speed_mbytes_per_sec = (float)(((double)mTotal) / (1000.0 * 1000.0)) / time_diff_s;
		log_printf("Average speed %.1f MBytes/Sec, Total=%zu Bytes/%zu MBytes\n", speed_mbytes_per_sec, mTotal, (mTotal / (1000 * 1000)));
		log_printf("End USB_TestDataIntegrity(libusb_bulk_transfer) EP%d Tests\n", ep_num);
	}

	return 0;
}

/* Return 0 if success other value are error code */
int USB_TestDataSizeIntegrity(struct libusb_device_handle* handle, uint32_t* writebuf, uint32_t* readbuf, int test_num)
{
	int test_data_len_max;
	int test_data_len;
	size_t mTotal;
	uint32_t test_data_start_val = 0;
	int ep_num;
	int mTestCount;
	double speed_mbytes_per_sec;
	float time_diff_s;

	struct timeval start_data;
	struct timeval curr_data;

	if (usb_speed == LIBUSB_SPEED_SUPER)
	{
		test_data_len_max = USB3_BULK_BURST_SIZE; /* USB3_BULK_BURST_SIZE Bytes for USB3 SS */
	}
	else
	{
		test_data_len_max = USB2_BULK_SIZE; /* 512 Bytes for USB2 HS */
	}

	for (ep_num = 1; ep_num < 8; ep_num++)
	{
		mTotal = 0;
		int bulk_ep_out = (ep_num | LIBUSB_ENDPOINT_OUT);
		int bulk_ep_in = (ep_num | LIBUSB_ENDPOINT_IN);

		log_printf("\nStart USB_TestDataSizeIntegrity(libusb_bulk_transfer) EP%d Tests\n", ep_num);

		log_printf("libusb_bulk_transfer(WriteEP=0x%02X)/libusb_bulk_transfer(ReadEP=0x%02X)/memcmp: %d times 1 to %d Bytes ***\n", bulk_ep_out, bulk_ep_in, test_num, test_data_len_max);

		log_printf_dbg("Start Test\n");
		gettimeofday(&start_data, NULL);
		for (mTestCount = 0; mTestCount < test_num; ++mTestCount) // loop test
		{
			for (test_data_len = 1; test_data_len < test_data_len_max; test_data_len++)
			{
				uint8_t* memWBuf = (uint8_t*)writebuf;
				for (uint32_t i = 0; i < test_data_len; i++)
				{
					memWBuf[i] = (i + test_data_start_val);
				}
				test_data_start_val += test_data_len;

				if (usb_write_EPx(handle, bulk_ep_out, (uint8_t*)writebuf, test_data_len) == 1)
				{
					mTotal += test_data_len;
				}
				else
				{
					log_printf("usb_write_EPx() error\n");
					log_printf("Test failure end with error\n");
					return -2;
				}

				memset(readbuf, 0x00, test_data_len);
				if (usb_read_EPx(handle, bulk_ep_in, (uint8_t*)readbuf, test_data_len) == 1)
				{
					mTotal += test_data_len;
				}
				else
				{
					log_printf("usb_read_EPx() error\n");
					log_printf("Test failure end with error\n");
					return -2;
				}

				if (memcmp(readbuf, writebuf, test_data_len) != 0)
				{
					log_printf("memcmp() failed mTestCount=%d\n", mTestCount);
					log_printf("Test failure end with error\n");
					return -2;
				}
			}
		}
		gettimeofday(&curr_data, NULL);
		log_printf_dbg("End Test\n");
		time_diff_s = TimevalDiff(&curr_data, &start_data);
		speed_mbytes_per_sec = (float)(((double)mTotal) / (1000.0 * 1000.0)) / time_diff_s;
		log_printf("Average speed %.1f MBytes/Sec, Total=%zu Bytes/%zu MBytes\n", speed_mbytes_per_sec, mTotal, (mTotal / (1000 * 1000)));
		log_printf("End USB_TestDataSizeIntegrity(libusb_bulk_transfer) EP%d Tests\n", ep_num);
	}

	return 0;
}

/* Return 0 if success other value are error code */
int USB_TestDataSpeed(struct libusb_device_handle* handle, uint32_t* writebuf, uint32_t* readbuf, int test_num, int test_data_len)
{
	int i;
	int test_data_len_max;
	size_t mTotal;
	uint32_t test_data_start_val = 0x11001100;
	int ep_num;
	int mTestCount;
	double speed_mbytes_per_sec;

	struct timeval start_data;
	struct timeval curr_data;

	float time_diff_s;

	uint8_t* memWBuf;

	if (usb_speed == LIBUSB_SPEED_SUPER)
	{
		test_data_len_max = USB3_BULK_BURST_SIZE; /* USB3_BULK_BURST_SIZE Bytes for USB3 SS */
	}
	else
	{
		test_data_len_max = USB2_BULK_SIZE; /* 512 Bytes for USB2 HS */
	}

	for (uint32_t i = 0; i < test_data_len / 4; i++)
	{
		writebuf[i] = swap_uint32(i + test_data_start_val); // Swap from LE to BE
	}

	for (ep_num = 1; ep_num < 8; ep_num++)
	{
		int bulk_ep_out = (ep_num | LIBUSB_ENDPOINT_OUT);
		int bulk_ep_in = (ep_num | LIBUSB_ENDPOINT_IN);

		log_printf("Start USB_TestDataSpeed(libusb_bulk_transfer) Tests\n");
		log_printf("libusb_bulk_transfer(WriteEP=0x%02X)/libusb_bulk_transfer(ReadEP=0x%02X): %d times %d Bytes ***\n", bulk_ep_out, bulk_ep_in, test_num, ((test_data_len / test_data_len_max) * test_data_len_max));

		log_printf_dbg("Start Test1 USB Bulk Write(libusb_bulk_transfer) EP%d\n", ep_num);
		mTotal = 0;
		gettimeofday(&start_data, NULL);
		for (mTestCount = 0; mTestCount < test_num; ++mTestCount) // loop test
		{
			memWBuf = (uint8_t*)writebuf;
			for (i = 0; i < (test_data_len / test_data_len_max); i++)
			{
				if (usb_write_EPx(handle, bulk_ep_out, memWBuf, test_data_len_max) == 1)
				{
					mTotal += test_data_len_max;
					memWBuf += test_data_len_max;
				}
				else
				{
					log_printf_dbg("usb_write_EPx() error\n");
					log_printf_dbg("Test failure end with error\n");
					return -2;
				}
			}
		}
		gettimeofday(&curr_data, NULL);
		log_printf_dbg("End Test\n");

		time_diff_s = TimevalDiff(&curr_data, &start_data);
		speed_mbytes_per_sec = (float)(((double)mTotal) / (1000.0 * 1000.0)) / time_diff_s;
		log_printf("Average speed %.1f MBytes/Sec, Total=%zu Bytes/%zu MBytes\n", speed_mbytes_per_sec, mTotal, (mTotal / (1000 * 1000)));
		/*
		log_printf_dbg("Start Test2 USB Bulk Read(libusb_bulk_transfer) EP%d\n", ep_num);
		mTotal = 0;
		gettimeofday(&start_data, NULL);
		for (mTestCount = 0; mTestCount < test_num; ++mTestCount) // loop test
		{
			for(i = 0; i < (test_data_len / test_data_len_max); i++)
			{
				if(usb_read_EPx(handle, bulk_ep_in, (uint8_t*)readbuf, test_data_len_max) == 1)
				{
					mTotal += test_data_len_max;
				}
				else
				{
					log_printf_dbg("usb_read_EPx() error\n");
					log_printf_dbg("Test failure end with error\n");
					return -2;
				}
			}
		}
		gettimeofday(&curr_data, NULL);
		log_printf_dbg("End Test\n");

		time_diff_s = TimevalDiff(&curr_data, &start_data);
		speed_mbytes_per_sec = (float)(((double)mTotal) / (1000.0 * 1000.0)) / time_diff_s;
		log_printf("Average speed %.1f MBytes/Sec, Total=%zu Bytes/%zu MBytes\n", speed_mbytes_per_sec, mTotal, (mTotal/(1000 * 1000)));
*/
		log_printf("End USB_TestDataSpeed(libusb_bulk_transfer) EP%d Tests\n", ep_num);
	}
	return 0;
}

/**
 * This is a custom project configuration structure where you can store the
 * parsed information.
 */
struct cag_configuration
{
	int verbose;
};

int main(int argc, char* argv[])
{
	char identifier;
	const char* value;
	cag_option_context context;
	struct cag_configuration config = { false }; /* Default values for the option(s) */

#define DATETIME_STR_SIZE (30)
	char datetime_str[DATETIME_STR_SIZE + 1] = "";
#define FILENAME_SIZE (30 + 100)
	char filename[FILENAME_SIZE + 1] = "";
	time_t rawtime;
	struct tm* timeinfo;

	/**
	* Prepare the context and iterate over all options.
	*/
	cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
	while (cag_option_fetch(&context))
	{
		identifier = cag_option_get(&context);
		switch (identifier)
		{
		case 'v':
			value = cag_option_get_value(&context);
			config.verbose = atoi(value);
			break;
		case 'h':
			printf("Usage: hydradancer_usb_benchmark [OPTION]...\n");
			cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
			return EXIT_SUCCESS;
		}
	}

#ifdef _WIN32
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
	{
		fprintf(stderr, "\nERROR: Could not set control handler\n");
		fflush(stderr);

		if (stdout)
		{
			fprintf(stdout, "\nERROR: Could not set control handler\n");
			fflush(stdout);
		}
		exit(-1);
	}
#else
	signal(SIGINT, consoleHandler);
#endif

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(datetime_str, DATETIME_STR_SIZE, "%Y%m%d_%H%M%S", timeinfo);
	snprintf(filename, FILENAME_SIZE, "hydrausb3_usb_benchmark_%s.txt", datetime_str);
	pFile = fopen(filename, "w");
	if (pFile == NULL)
	{
		fprintf(stderr, "fopen(filename, \"w\") error (filename=\"%s\")\n", filename);
		fflush(stderr);
		error_exit(NULL);
	}
	log_printf_init(pFile);
	log_printf("hydradancer_usb_benchmark B.VERNOUX v%s\n", VERSION);

	log_printf("Options: verbose=%d\n",
			   config.verbose);

	if (usb_init() < 0)
	{
		error_exit("usb_init() error exit\n");
	}

	handle = usb_opendev(config.verbose);
	if (handle == NULL)
	{
		error_exit("usb_opendev() error exit\n");
	}
	usb_speed = usb_get_device_speed(handle);
	/*
	if(usb_speed == LIBUSB_SPEED_SUPER)
	{
		log_printf("Test end with success\n");
	}
	else
	{
		error_exit("Error HydraUSB3 device shall be connected with SuperSpeed 5Gbit/s Computer/Cable(no HUB) to start the tests\n");
	}
*/
	/* USB3 SS Test Data Size (1 to max) Integrity */
	if (USB_TestDataSizeIntegrity(handle, mWriteBigBuf, mReadBigBuf, TEST_INTEGRITY_SIZE_NUM) != 0)
	{
		error_exit(NULL);
	}
	log_printf("\n");

	/* USB3 SS Test Data Integrity */
	if (USB_TestDataIntegrity(handle, mWriteBigBuf, mReadBigBuf, TEST_INTEGRITY_NUM) != 0)
	{
		error_exit(NULL);
	}
	log_printf("\n");

	/* USB3 SS Test Data Speed */
	/*
	if(USB_TestDataSpeed(handle, mWriteBigBuf, mReadBigBuf, TEST_NUM_SS, TEST_DATA_LEN) != 0)
	{
		error_exit(NULL);
	}
	log_printf("\n");
	*/

	/* Summary */
	log_printf("Tests end with success\n");

	cleanup();

	return EXIT_SUCCESS;
}
