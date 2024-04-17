#include <libusb-1.0/libusb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define VID (uint16_t)0x16c0
#define PID (uint16_t)0x27d8

// Standard control requests codes go from 0-27 and 48-49
#define ENABLE_USB_CONNECTION_REQUEST_CODE 50
#define SET_ADDRESS_REQUEST_CODE 51
#define GET_hydradancer_status 52
#define SET_ENDPOINT_MAPPING 53
#define DISABLE_USB 54
#define SET_SPEED 55
#define SET_EP_RESPONSE 56

#define USB2_LS 0x00
#define USB2_FS 0x01
#define USB2_HS 0x02

uint8_t endpoints_mapping[6] = { 1, 2, 3, 4, 5, 6 };
uint8_t reverse_endpoints_mapping[7] = { 0, 0, 2, 2, 3, 4, 5 };
libusb_device_handle* handle = NULL;

typedef struct __attribute__((packed))
{
	uint8_t ep_in_status;
	uint8_t ep_out_status;
	uint8_t ep_in_nak;
	uint8_t other_events;
} hydradancer_status_t;

hydradancer_status_t hydradancer_status;

uint8_t ep1_rx_buffer[512];
uint8_t ep1_tx_buffer[512];
uint8_t log_buffer[512];

void list_product_string(void);
void list_product_string(void)
{
	libusb_context* context = NULL;
	libusb_init(&context);
	libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

	libusb_device** devices = NULL;
	libusb_get_device_list(context, &devices);

	libusb_device** devices_ptr = devices;

	while (*devices_ptr != NULL)
	{
		libusb_open(*devices_ptr, &handle);

		struct libusb_device_descriptor desc;
		libusb_get_device_descriptor(*devices_ptr, &desc);

		char buffer[200];
		libusb_get_string_descriptor_ascii(handle, desc.iProduct, (uint8_t*)buffer, 200);
		printf("%s\r\n", buffer);

		libusb_close(handle);
		++devices_ptr;
	}

	libusb_free_device_list(devices, 1);
	libusb_exit(context);
}

void ep1_handle_data_available(uint8_t* buffer, size_t length);
void ep1_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep1\r\n", length);
}

int send(uint8_t ep_num, uint8_t* buffer, size_t size);
int send(uint8_t ep_num, uint8_t* buffer, size_t size)
{
	int num_bytes_transferred = 0;
	libusb_bulk_transfer(handle, endpoints_mapping[ep_num], buffer, size, &num_bytes_transferred, 0);
	printf("transferred %d bytes \r\n", num_bytes_transferred);
	hydradancer_status.ep_in_status &= ~(1 << ep_num);
	return num_bytes_transferred;
}

void ep1_handle_data_requested(void);
void ep1_handle_data_requested(void)
{
	printf("data requested on ep1\r\n");
	send(1, ep1_tx_buffer, sizeof(ep1_tx_buffer));
}

libusb_device* usb_get_device(libusb_context* context, uint16_t vid, uint16_t pid);
libusb_device* usb_get_device(libusb_context* context, uint16_t vid, uint16_t pid)
{
	libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

	libusb_device** devices = NULL;
	libusb_device* result = NULL;
	libusb_get_device_list(context, &devices);

	libusb_device** devices_ptr = devices;

	while (*devices_ptr != NULL)
	{
		libusb_open(*devices_ptr, &handle);
		struct libusb_device_descriptor desc;
		libusb_get_device_descriptor(*devices_ptr, &desc);
		libusb_close(handle);

		if (desc.idVendor == vid && desc.idProduct == pid)
		{
			result = *devices_ptr;
		}
		else
		{
			libusb_unref_device(*devices_ptr);
		}
		++devices_ptr;
	}

	libusb_free_device_list(devices, 0);
	return result;
}

void prepare_tx_buffer(void);
void prepare_tx_buffer(void)
{
	for (size_t i = 0; i < sizeof(ep1_tx_buffer); ++i)
	{
		ep1_tx_buffer[i] = i;
	}
}

int main()
{
	prepare_tx_buffer();

	libusb_context* context = NULL;
	libusb_init(&context);
	libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
	libusb_device* usb_device = usb_get_device(context, VID, PID);

	if (usb_device == NULL)
	{
		goto exit_libusb;
	}

	libusb_open(usb_device, &handle);

	// set PHY speed to high-speed
	libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), SET_SPEED, USB2_HS, 0, NULL, 0, 0);
	// enable USB on emulated board
	libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), ENABLE_USB_CONNECTION_REQUEST_CODE, 0, 0, NULL, 0, 0);

	// Set endpoints mapping
	for (size_t i = 0; i < sizeof(endpoints_mapping); ++i)
	{
		libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), SET_ENDPOINT_MAPPING, (i & 0x00ff) | ((endpoints_mapping[i] << 8) & 0xff00), 0, NULL, 0, 0);
	}

	libusb_close(handle);
	libusb_unref_device(usb_device);

	// SET_CONFIGURATION will reset the state of the USB2 peripheral on the emulated board, this prevents sending data before that
	libusb_device* emulated_device = NULL;
	do {
		emulated_device = usb_get_device(context, 0x610b, 0x4653);
	} while (!emulated_device);

	// somehow my handle on hydradancer gets invalidate, so recreate one
	usb_device = usb_get_device(context, VID, PID);
	libusb_open(usb_device, &handle);

	// Poll ep6 and execute callbacks
	bool end_polling = false;

	printf("Start polling \r\n");

	while (!end_polling)
	{
		hydradancer_status_t hydradancer_status_new = { .ep_in_status = 0, .ep_out_status = 0 };
		libusb_bulk_transfer(handle, (1 << 7) | 1, (uint8_t*)(&hydradancer_status_new), sizeof(hydradancer_status_new), NULL, 1);
		hydradancer_status.ep_in_status |= hydradancer_status_new.ep_in_status;
		hydradancer_status.ep_out_status |= hydradancer_status_new.ep_out_status;

		// int num_bytes_log = 0;
		// // libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), GET_LOGS, 0, 0, NULL, 0, 1);
		// libusb_bulk_transfer(handle, (1 << 7) | 2, log_buffer, sizeof(log_buffer), &num_bytes_log, 1);

		// if(num_bytes_log > 0){
		//     printf("%s \r\n", log_buffer);
		// }

		if (hydradancer_status.ep_out_status & (1 << 1))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[1], ep1_rx_buffer, sizeof(ep1_rx_buffer), &num_bytes_received, 0);
			ep1_handle_data_available(ep1_rx_buffer, num_bytes_received);
			hydradancer_status.ep_out_status &= ~(1 << 1);
		}

		if (hydradancer_status.ep_in_status & (1 << 1))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep1_handle_data_requested();
		}
	}

	libusb_close(handle);
	libusb_unref_device(usb_device);
exit_libusb:
	libusb_exit(context);
}
