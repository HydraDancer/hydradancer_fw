#ifndef USB_DEVICE_USER_H
#define USB_DEVICE_USER_H

#define ENDP_1_15_MAX_PACKET_SIZE 512

#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_device.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

/* Global define */
// DEF_ENDP_OUT_BURST_LEVEL / DEF_ENDP_IN_BURST_LEVEL maximum burst size 16 defined by the USB3 specification
// Warning USB3 endpoint bulk with 8 or 16 burst can be problematic on some PC
#define DEF_ENDP_OUT_BURST_LEVEL 1
#define DEF_ENDP_IN_BURST_LEVEL (DEF_ENDP_OUT_BURST_LEVEL)
#define DEF_ENDP_MAX_SIZE (DEF_ENDP1_OUT_BURST_LEVEL * 1024)

__attribute__((aligned(16))) uint8_t endp0_buffer[512] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp1_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp1_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp2_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp2_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp3_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp3_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp4_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp4_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp5_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp5_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp6_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp6_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

__attribute__((aligned(16))) uint8_t endp7_tx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) uint8_t endp7_rx_buffer[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL] __attribute__((section(".DMADATA")));

uint8_t hydradancer_product_string_descriptor[] = {
	'H',
	0x00,
	'y',
	0x00,
	'd',
	0x00,
	'r',
	0x00,
	'a',
	0x00,
	'd',
	0x00,
	'a',
	0x00,
	'n',
	0x00,
	'c',
	0x00,
	'e',
	0x00,
	'r',
	0x00,
	' ',
	0x00,
	'a',
	0x00,
	' ',
	0x00,
	'F',
	0x00,
	'a',
	0x00,
	'c',
	0x00,
	'e',
	0x00,
	'd',
	0x00,
	'a',
	0x00,
	'n',
	0x00,
	'c',
	0x00,
	'e',
	0x00,
	'r',
	0x00,
	' ',
	0x00,
	'B',
	0x00,
	'a',
	0x00,
	'c',
	0x00,
	'k',
	0x00,
	'e',
	0x00,
	'n',
	0x00,
	'd',
	0x00,
	' ',
	0x00,
};

struct usb_string_descriptors
{
	USB_STRING_DESCR lang_ids_descriptor;
	uint16_t lang_ids[1];
	USB_STRING_DESCR product_string_descriptor;
	uint8_t hydradancer_product_string_descriptor[sizeof(hydradancer_product_string_descriptor)];
} usb_string_descriptors;

const USB_STRING_DESCR* device_string_descriptors[2];

void init_string_descriptors(void);
void init_string_descriptors(void)
{
	usb_string_descriptors.lang_ids_descriptor = (USB_STRING_DESCR){
		.bLength = sizeof(USB_STRING_DESCR) + sizeof(usb_string_descriptors.lang_ids),
		.bDescriptorType = 0x03, //String Descriptor
	};

	usb_string_descriptors.lang_ids[0] = 0x0409;

	usb_string_descriptors.product_string_descriptor = (USB_STRING_DESCR){
		.bLength = sizeof(USB_STRING_DESCR) + sizeof(hydradancer_product_string_descriptor),
		.bDescriptorType = 0x03, //String Descriptor
	};

	memcpy(&usb_string_descriptors.hydradancer_product_string_descriptor, hydradancer_product_string_descriptor, sizeof(hydradancer_product_string_descriptor));

	device_string_descriptors[0] = &usb_string_descriptors.lang_ids_descriptor;
	device_string_descriptors[1] = &usb_string_descriptors.product_string_descriptor;
}

void init_endpoints(void);
void init_endpoints(void)
{
	usb_device_0.endpoints.rx[0].buffer = endp0_buffer;
	usb_device_0.endpoints.rx[0].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[0].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[0].max_packet_size_with_burst = sizeof(endp0_buffer);

	usb_device_0.endpoints.rx[1].buffer = endp1_rx_buffer;
	usb_device_0.endpoints.rx[1].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[1].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[1].max_packet_size_with_burst = sizeof(endp1_rx_buffer);

	usb_device_0.endpoints.tx[1].buffer = NULL;
	usb_device_0.endpoints.tx[1].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[1].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[1].max_packet_size_with_burst = sizeof(endp1_rx_buffer);

	usb_device_0.endpoints.rx[2].buffer = endp2_rx_buffer;
	usb_device_0.endpoints.rx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[2].max_packet_size_with_burst = sizeof(endp2_rx_buffer);

	usb_device_0.endpoints.tx[2].buffer = NULL;
	usb_device_0.endpoints.tx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[2].max_packet_size_with_burst = sizeof(endp2_rx_buffer);

	usb_device_0.endpoints.rx[3].buffer = endp3_rx_buffer;
	usb_device_0.endpoints.rx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[3].max_packet_size_with_burst = sizeof(endp3_rx_buffer);

	usb_device_0.endpoints.tx[3].buffer = NULL;
	usb_device_0.endpoints.tx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[3].max_packet_size_with_burst = sizeof(endp3_rx_buffer);

	usb_device_0.endpoints.rx[4].buffer = endp4_rx_buffer;
	usb_device_0.endpoints.rx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[4].max_packet_size_with_burst = sizeof(endp4_rx_buffer);

	usb_device_0.endpoints.tx[4].buffer = NULL;
	usb_device_0.endpoints.tx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[4].max_packet_size_with_burst = sizeof(endp4_rx_buffer);

	usb_device_0.endpoints.rx[5].buffer = endp5_rx_buffer;
	usb_device_0.endpoints.rx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[5].max_packet_size_with_burst = sizeof(endp5_rx_buffer);

	usb_device_0.endpoints.tx[5].buffer = NULL;
	usb_device_0.endpoints.tx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[5].max_packet_size_with_burst = sizeof(endp5_rx_buffer);

	usb_device_0.endpoints.rx[6].buffer = endp6_rx_buffer;
	usb_device_0.endpoints.rx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[6].max_packet_size_with_burst = sizeof(endp6_rx_buffer);

	usb_device_0.endpoints.tx[6].buffer = NULL;
	usb_device_0.endpoints.tx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[6].max_packet_size_with_burst = sizeof(endp6_rx_buffer);

	usb_device_0.endpoints.rx[7].buffer = endp7_rx_buffer;
	usb_device_0.endpoints.rx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[7].max_packet_size_with_burst = sizeof(endp7_rx_buffer);

	usb_device_0.endpoints.tx[7].buffer = NULL;
	usb_device_0.endpoints.tx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[7].max_packet_size_with_burst = sizeof(endp7_rx_buffer);
}

#endif
