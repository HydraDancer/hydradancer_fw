/********************************** (C) COPYRIGHT *******************************
* File Name          : libusb_portable.h
* Author             : bvernoux
* Version            : V0.1.0
* Date               : 2023/08/06
* Description        :
* Copyright (c) 2023 Benjamin VERNOUX
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#ifndef __LIBUSB_PORTABLE_H__
#define __LIBUSB_PORTABLE_H__

#include "libusb.h"
#include <sys/types.h>

#ifndef _WIN32
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LIBUSB_BULK_TRANSFER_TIMEOUT_MILLISEC (2000)

float TimevalDiff(const struct timeval* a, const struct timeval* b);
void get_CurrentTime(char* date_time_ms, int date_time_ms_max_size);

void sleep_ms(int milliseconds); // Cross-platform sleep in milliseconds function
void sleep_us(int microseconds); // Cross-platform sleep in microseconds function

void print_hex(uint8_t* data, uint16_t size);

void log_printf_init(FILE* log_file_handle); /* Required before to call log_printf_dbg()/log_printf() API */

void log_printf_dbg(const char* fmt, ...);
void log_printf(const char* fmt, ...);

// return 0 if ok or error code
int usb_init(void);
/*
Return 0 if error or struct libusb_device_handle * if success
*/
struct libusb_device_handle* usb_opendev(int verbose);

void usb_closedev(struct libusb_device_handle* libusb_dev_handle);

void usb_exit(void);

/*
Return usb speed see enum libusb_speed
*/
enum libusb_speed usb_get_device_speed(libusb_device_handle* handle);

/*
   return 1 if ok or error code
*/
int usb_write_EPx(libusb_device_handle* handle, int ep_out, uint8_t* data_tx, uint16_t data_tx_size);

/*
   return 1 if ok or error code
*/
int usb_read_EPx(libusb_device_handle* handle, int ep_in, uint8_t* data_rx, uint16_t data_rx_size);

#ifdef __cplusplus
}
#endif

#endif /* __LIBUSB_PORTABLE_H__ */