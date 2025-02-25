#include "usb_control_device.h"
#include "version.h"

uint8_t hydradancer_manufacturer_string_descriptor[] = {
	'Q', 0x00, 'u', 0x00, 'a', 0x00, 'r', 0x00, 'k', 0x00, 's', 0x00, 'l', 0x00, 'a', 0x00, 'b', 0x00, ' ', 0x00, 'h', 0x00, 't', 0x00, 't', 0x00, 'p', 0x00, 's', 0x00, ':', 0x00, '/', 0x00, '/', 0x00, 'w', 0x00, 'w', 0x00, 'w', 0x00, '.', 0x00, 'q', 0x00, 'u', 0x00, 'a', 0x00, 'r', 0x00, 'k', 0x00, 's', 0x00, 'l', 0x00, 'a', 0x00, 'b', 0x00, '.', 0x00, 'c', 0x00, 'o', 0x00, 'm', 0x00, '/', 0x00, ' ', 0x00, '&', 0x00, ' ', 0x00, 'H', 0x00, 'y', 0x00, 'd', 0x00, 'r', 0x00, 'a', 0x00, 'B', 0x00, 'u', 0x00, 's', 0x00, ' ', 0x00, 'h', 0x00, 't', 0x00, 't', 0x00, 'p', 0x00, 's', 0x00, ':', 0x00, '/', 0x00, '/', 0x00, 'h', 0x00, 'y', 0x00, 'd', 0x00, 'r', 0x00, 'a', 0x00, 'b', 0x00, 'u', 0x00, 's', 0x00, '.', 0x00, 'c', 0x00, 'o', 0x00, 'm', 0x00, '/', 0x00
};

struct usb_string_descriptors
{
	USB_STRING_DESCR lang_ids_descriptor;
	uint16_t lang_ids[1];
	USB_STRING_DESCR product_string_descriptor;
	uint8_t hydradancer_product_string_descriptor[sizeof(hydradancer_product_string_descriptor)];
	USB_STRING_DESCR manufacturer_string_descriptor;
	uint8_t hydradancer_manufacturer_string_descriptor[sizeof(hydradancer_manufacturer_string_descriptor)];
} usb_string_descriptors;

const USB_STRING_DESCR* device_string_descriptors[3];

void usb_control_init_string_descriptors(void)
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

	usb_string_descriptors.manufacturer_string_descriptor = (USB_STRING_DESCR){
		.bLength = sizeof(USB_STRING_DESCR) + sizeof(hydradancer_manufacturer_string_descriptor),
		.bDescriptorType = 0x03, //String Descriptor
	};

	memcpy(&usb_string_descriptors.hydradancer_product_string_descriptor, hydradancer_product_string_descriptor, sizeof(hydradancer_product_string_descriptor));
	memcpy(&usb_string_descriptors.hydradancer_manufacturer_string_descriptor, hydradancer_manufacturer_string_descriptor, sizeof(hydradancer_manufacturer_string_descriptor));

	device_string_descriptors[0] = &usb_string_descriptors.lang_ids_descriptor;
	device_string_descriptors[1] = &usb_string_descriptors.product_string_descriptor;
	device_string_descriptors[2] = &usb_string_descriptors.manufacturer_string_descriptor;
}

void usb_control_init_endpoints(void)
{
	usb_device_1.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(512);
	usb_device_1.endpoints.rx[0].max_packet_size = 512;
	usb_device_1.endpoints.rx[0].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[0].max_packet_size_with_burst = 512;
	usb_device_1.endpoints.rx[0].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[1].buffer = NULL;
	usb_device_1.endpoints.tx[1].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[1].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[1].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[1].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[2].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[2].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[2].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[2].buffer = NULL;
	usb_device_1.endpoints.tx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[2].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[2].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[3].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[3].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[3].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[3].buffer = NULL;
	usb_device_1.endpoints.tx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[3].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[3].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[4].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[4].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[4].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[4].buffer = NULL;
	usb_device_1.endpoints.tx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[4].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[4].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[5].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[5].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[5].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[5].buffer = NULL;
	usb_device_1.endpoints.tx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[5].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[5].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[6].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[6].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[6].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[6].buffer = NULL;
	usb_device_1.endpoints.tx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[6].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[6].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.rx[7].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_1.endpoints.rx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.rx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[7].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.rx[7].state = ENDP_STATE_ACK;

	usb_device_1.endpoints.tx[7].buffer = NULL;
	usb_device_1.endpoints.tx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_1.endpoints.tx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[7].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_1.endpoints.tx[7].state = ENDP_STATE_ACK;
}

bool usb_control_reinit(void)
{
	// // uncomment to empty log_buffer on emulated device disconnection
	// LOG_DUMP();
	BSP_ENTER_CRITICAL();
	usb_control_init_endpoints(); // update USB3 backend as well, without rebooting USB3 peripheral
	usb30_init_endpoints();
	BSP_EXIT_CRITICAL();
	return true;
}
