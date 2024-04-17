#include <libusb-1.0/libusb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define VID (uint16_t)0x16c0
#define PID (uint16_t)0x27d8

#define ENABLE_USB_CONNECTION_REQUEST_CODE 50
#define SET_ADDRESS_REQUEST_CODE 51
#define GET_IN_ENDPOINTS_STATUS 52
#define SET_ENDPOINT_MAPPING 53
#define GET_LOGS 54

uint8_t endpoints_mapping[8];
uint8_t reverse_endpoints_mapping[9];
libusb_device_handle* handle = NULL;

typedef struct __attribute__((packed))
{
	uint8_t ep_in_status;
	uint8_t ep_out_status;
	uint8_t ep_in_nak;
	uint8_t other_events;
} hydradancer_status_t;

uint8_t ep1_rx_buffer[512];
uint8_t ep2_rx_buffer[512];
uint8_t ep3_rx_buffer[512];
uint8_t ep4_rx_buffer[512];
uint8_t ep5_rx_buffer[512];
uint8_t ep6_rx_buffer[512];
uint8_t ep7_rx_buffer[512];
size_t ep1_rx_last_received_size = 0;
size_t ep2_rx_last_received_size = 0;
size_t ep3_rx_last_received_size = 0;
size_t ep4_rx_last_received_size = 0;
size_t ep5_rx_last_received_size = 0;
size_t ep6_rx_last_received_size = 0;
size_t ep7_rx_last_received_size = 0;
uint8_t log_buffer[512];
bool buffer_pending_1 = false;
bool buffer_pending_2 = false;
bool buffer_pending_3 = false;
bool buffer_pending_4 = false;
bool buffer_pending_5 = false;
bool buffer_pending_6 = false;
bool buffer_pending_7 = false;

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
	buffer_pending_1 = true;
	ep1_rx_last_received_size = length;
}

void ep2_handle_data_available(uint8_t* buffer, size_t length);
void ep2_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep2\r\n", length);
	buffer_pending_2 = true;
	ep2_rx_last_received_size = length;
}

void ep3_handle_data_available(uint8_t* buffer, size_t length);
void ep3_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep3\r\n", length);
	buffer_pending_3 = true;
	ep3_rx_last_received_size = length;
}

void ep4_handle_data_available(uint8_t* buffer, size_t length);
void ep4_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep4\r\n", length);
	buffer_pending_4 = true;
	ep4_rx_last_received_size = length;
}

void ep5_handle_data_available(uint8_t* buffer, size_t length);
void ep5_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep5\r\n", length);
	buffer_pending_5 = true;
	ep5_rx_last_received_size = length;
}

void ep6_handle_data_available(uint8_t* buffer, size_t length);
void ep6_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep6\r\n", length);
	buffer_pending_6 = true;
	ep6_rx_last_received_size = length;
}

void ep7_handle_data_available(uint8_t* buffer, size_t length);
void ep7_handle_data_available(uint8_t* buffer, size_t length)
{
	printf("received %ld bytes on ep7\r\n", length);
	buffer_pending_7 = true;
	ep7_rx_last_received_size = length;
}

int send(uint8_t ep_num, uint8_t* buffer, size_t size);
int send(uint8_t ep_num, uint8_t* buffer, size_t size)
{
	int num_bytes_transferred = 0;
	libusb_bulk_transfer(handle, endpoints_mapping[ep_num], buffer, size, &num_bytes_transferred, 0);
	printf("transferred %d bytes \r\n", num_bytes_transferred);
	return num_bytes_transferred;
}

void ep1_handle_data_requested(void);
void ep1_handle_data_requested(void)
{
	printf("data requested on ep1\r\n");
	if (buffer_pending_1)
	{
		send(1, ep1_rx_buffer, ep1_rx_last_received_size);
		buffer_pending_1 = false;
	}
}

void ep2_handle_data_requested(void);
void ep2_handle_data_requested(void)
{
	printf("data requested on ep2\r\n");
	if (buffer_pending_2)
	{
		send(2, ep2_rx_buffer, ep2_rx_last_received_size);
		buffer_pending_2 = false;
	}
}

void ep3_handle_data_requested(void);
void ep3_handle_data_requested(void)
{
	printf("data requested on ep3\r\n");
	if (buffer_pending_3)
	{
		send(3, ep3_rx_buffer, ep3_rx_last_received_size);
		buffer_pending_3 = false;
	}
}

void ep4_handle_data_requested(void);
void ep4_handle_data_requested(void)
{
	printf("data requested on ep4\r\n");
	if (buffer_pending_4)
	{
		send(4, ep4_rx_buffer, ep4_rx_last_received_size);
		buffer_pending_4 = false;
	}
}

void ep5_handle_data_requested(void);
void ep5_handle_data_requested(void)
{
	printf("data requested on ep5\r\n");
	if (buffer_pending_5)
	{
		send(5, ep5_rx_buffer, ep5_rx_last_received_size);
		buffer_pending_5 = false;
	}
}

void ep6_handle_data_requested(void);
void ep6_handle_data_requested(void)
{
	printf("data requested on ep6\r\n");
	if (buffer_pending_6)
	{
		send(6, ep6_rx_buffer, ep6_rx_last_received_size);
		buffer_pending_6 = false;
	}
}

void ep7_handle_data_requested(void);
void ep7_handle_data_requested(void)
{
	printf("data requested on ep7\r\n");
	if (buffer_pending_7)
	{
		send(7, ep7_rx_buffer, ep7_rx_last_received_size);
		buffer_pending_7 = false;
	}
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

int main()
{
	libusb_context* context = NULL;
	libusb_init(&context);
	libusb_set_option(context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
	libusb_device* usb_device = usb_get_device(context, VID, PID);

	if (usb_device == NULL)
	{
		goto exit_libusb;
	}

	libusb_open(usb_device, &handle);

	uint8_t activated_endpoints[] = { 1, 2, 3, 4 };
	uint8_t available_endpoints_hydradancer[] = { 2, 3, 4, 5 };
	// Set endpoints mapping
	for (size_t i = 0; i < sizeof(activated_endpoints); ++i)
	{
		endpoints_mapping[activated_endpoints[i]] = available_endpoints_hydradancer[i];
		libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), SET_ENDPOINT_MAPPING, (activated_endpoints[i] & 0x00ff) | ((endpoints_mapping[activated_endpoints[i]] << 8) & 0xff00), 0, NULL, 0, 0);
	}

	// enable USB on emulated board
	libusb_control_transfer(handle, (0 << 7 | 1 << 6 | 0x00), ENABLE_USB_CONNECTION_REQUEST_CODE, 0, 0, NULL, 0, 0);

	// Poll ep1 and execute callbacks

	bool end_polling = false;

	printf("Start polling \r\n");

	while (!end_polling)
	{
		hydradancer_status_t hydradancer_status = { .ep_in_status = 0, .ep_out_status = 0 };
		libusb_bulk_transfer(handle, (1 << 7) | 1, (uint8_t*)(&hydradancer_status), sizeof(hydradancer_status), NULL, 0);

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
		}

		if (hydradancer_status.ep_out_status & (1 << 2))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[2], ep2_rx_buffer, sizeof(ep2_rx_buffer), &num_bytes_received, 0);
			ep2_handle_data_available(ep2_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_out_status & (1 << 3))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[3], ep3_rx_buffer, sizeof(ep3_rx_buffer), &num_bytes_received, 0);
			ep3_handle_data_available(ep3_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_out_status & (1 << 4))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[4], ep4_rx_buffer, sizeof(ep4_rx_buffer), &num_bytes_received, 0);
			ep4_handle_data_available(ep4_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_out_status & (1 << 5))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[5], ep5_rx_buffer, sizeof(ep5_rx_buffer), &num_bytes_received, 0);
			ep5_handle_data_available(ep5_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_out_status & (1 << 6))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[6], ep6_rx_buffer, sizeof(ep6_rx_buffer), &num_bytes_received, 0);
			ep6_handle_data_available(ep6_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_out_status & (1 << 7))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			int num_bytes_received = 0;
			libusb_bulk_transfer(handle, (1 << 7) | endpoints_mapping[7], ep7_rx_buffer, sizeof(ep7_rx_buffer), &num_bytes_received, 0);
			ep7_handle_data_available(ep7_rx_buffer, num_bytes_received);
		}

		if (hydradancer_status.ep_in_status & (1 << 1))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep1_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 2))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep2_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 3))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep3_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 4))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep4_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 5))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep5_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 6))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep6_handle_data_requested();
		}
		if (hydradancer_status.ep_in_status & (1 << 7))
		{
			printf("received status in %d out %d \r\n", hydradancer_status.ep_in_status, hydradancer_status.ep_out_status);
			ep7_handle_data_requested();
		}
	}

	libusb_close(handle);
	libusb_unref_device(usb_device);
exit_libusb:
	libusb_exit(context);
}
