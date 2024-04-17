#include "usb_emulation_device.h"
#include "usb_control_device.h"

void usb_emulation_reinit(void)
{
	// // uncomment to empty log_buffer on emulated device disconnection
	// LOG_DUMP();

	bsp_disable_interrupt();
	// Setup ramx pool and interrupt_queue
	usb_device_set_usb3_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb2_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb3_config_descriptors(&usb_device_0, NULL);
	usb_device_set_usb2_config_descriptors(&usb_device_0, NULL);
	usb_device_set_bos_descriptor(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	// Set the USB device parameters
	usb2_ep0_passthrough_enabled(true);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);
	usb_emulation_init_endpoints();
	usb2_setup_endpoints();
	usb2_set_device_address(0);
	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}
	bsp_enable_interrupt();
}

void usb_emulation_init_endpoints(void)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[0].max_packet_size = 64;
	usb_device_0.endpoints.rx[0].max_burst = 0;
	usb_device_0.endpoints.rx[0].max_packet_size_with_burst = 64;

	usb_device_0.endpoints.tx[0].buffer = usb_device_0.endpoints.rx[0].buffer;
	usb_device_0.endpoints.tx[0].max_packet_size = 64;
	usb_device_0.endpoints.tx[0].max_burst = 0;
	usb_device_0.endpoints.tx[0].max_packet_size_with_burst = 64;

	usb_device_0.endpoints.rx[1].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[1].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[1].max_burst = 0;
	usb_device_0.endpoints.rx[1].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[1].buffer = NULL;
	usb_device_0.endpoints.tx[1].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[1].max_burst = 0;
	usb_device_0.endpoints.tx[1].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[2].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[2].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[2].max_burst = 0;
	usb_device_0.endpoints.rx[2].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[2].buffer = NULL;
	usb_device_0.endpoints.tx[2].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[2].max_burst = 0;
	usb_device_0.endpoints.tx[2].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[3].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[3].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[3].max_burst = 0;
	usb_device_0.endpoints.rx[3].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[3].buffer = NULL;
	usb_device_0.endpoints.tx[3].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[3].max_burst = 0;
	usb_device_0.endpoints.tx[3].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[4].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[4].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[4].max_burst = 0;
	usb_device_0.endpoints.rx[4].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[4].buffer = NULL;
	usb_device_0.endpoints.tx[4].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[4].max_burst = 0;
	usb_device_0.endpoints.tx[4].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[5].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[5].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[5].max_burst = 0;
	usb_device_0.endpoints.rx[5].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[5].buffer = NULL;
	usb_device_0.endpoints.tx[5].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[5].max_burst = 0;
	usb_device_0.endpoints.tx[5].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[6].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[6].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[6].max_burst = 0;
	usb_device_0.endpoints.rx[6].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[6].buffer = NULL;
	usb_device_0.endpoints.tx[6].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[6].max_burst = 0;
	usb_device_0.endpoints.tx[6].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[7].buffer = ramx_pool_alloc_bytes(USB2_ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[7].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[7].max_burst = 0;
	usb_device_0.endpoints.rx[7].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[7].buffer = NULL;
	usb_device_0.endpoints.tx[7].max_packet_size = USB2_ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[7].max_burst = 0;
	usb_device_0.endpoints.tx[7].max_packet_size_with_burst = USB2_ENDP_1_15_MAX_PACKET_SIZE;
}

void usb_emulation_do_bus_reset(void)
{
	bsp_disable_interrupt();
	usb_device_set_usb3_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb2_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb3_config_descriptors(&usb_device_0, NULL);
	usb_device_set_usb2_config_descriptors(&usb_device_0, NULL);
	usb_device_set_bos_descriptor(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	// Set the USB device parameters
	usb2_ep0_passthrough_enabled(true);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);
	usb_emulation_init_endpoints();
	usb2_setup_endpoints();
	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}
	bsp_enable_interrupt();
}
