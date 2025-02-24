#ifndef USB_HANDLERS_H
#define USB_HANDLERS_H

#include "definitions.h"
#include "wch-ch56x-lib/hspi_scheduled/hspi_scheduled.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/usb/usb30.h"
#include "wch-ch56x-lib/usb/usb_descriptors.h"
#include "wch-ch56x-lib/usb/usb_endpoints.h"

void endp1_tx_complete(TRANSACTION_STATUS status);
void endp1_tx_complete(TRANSACTION_STATUS status)
{
	if (hydradancer_status.other_events != 0)
	{
		hydradancer_status.other_events = 0; // only send events once
	}
}

void endp2_tx_complete(TRANSACTION_STATUS status);
void endp2_tx_complete(TRANSACTION_STATUS status)
{
	ramx_pool_free(usb_device_0.endpoints.tx[2].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[2]);
	ep_status_buffer[0] = endpoint_mapping_reverse[2];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}
void endp3_tx_complete(TRANSACTION_STATUS status);
void endp3_tx_complete(TRANSACTION_STATUS status)
{
	ramx_pool_free(usb_device_0.endpoints.tx[3].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[3]);
	ep_status_buffer[0] = endpoint_mapping_reverse[3];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}
void endp4_tx_complete(TRANSACTION_STATUS status);
void endp4_tx_complete(TRANSACTION_STATUS status)
{
	ramx_pool_free(usb_device_0.endpoints.tx[4].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[4]);
	ep_status_buffer[0] = endpoint_mapping_reverse[4];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}
void endp5_tx_complete(TRANSACTION_STATUS status);
void endp5_tx_complete(TRANSACTION_STATUS status)
{
	ramx_pool_free(usb_device_0.endpoints.tx[5].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[5]);
	ep_status_buffer[0] = endpoint_mapping_reverse[5];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp6_tx_complete(TRANSACTION_STATUS status);
void endp6_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp6_tx_complete buffer %p %d %d %d %d %d \r\n", usb_device_0.endpoints.tx[6].buffer, usb_device_0.endpoints.tx[6].buffer[0], usb_device_0.endpoints.tx[6].buffer[1], usb_device_0.endpoints.tx[6].buffer[2], usb_device_0.endpoints.tx[6].buffer[3], usb_device_0.endpoints.tx[6].buffer[4]);

	ramx_pool_free(usb_device_0.endpoints.tx[6].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[6]);
	ep_status_buffer[0] = endpoint_mapping_reverse[6];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp7_tx_complete(TRANSACTION_STATUS status);
void endp7_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp7_tx_complete buffer %p %d %d %d %d %d \r\n", usb_device_0.endpoints.tx[7].buffer, usb_device_0.endpoints.tx[7].buffer[0], usb_device_0.endpoints.tx[7].buffer[1], usb_device_0.endpoints.tx[7].buffer[2], usb_device_0.endpoints.tx[7].buffer[3], usb_device_0.endpoints.tx[7].buffer[4]);

	ramx_pool_free(usb_device_0.endpoints.tx[7].buffer);
	hydradancer_status.ep_out_status &= ~(0x01 << endpoint_mapping_reverse[7]);
	ep_status_buffer[0] = endpoint_mapping_reverse[7];
	ep_status_buffer[1] = 0;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_OUT_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

uint8_t endp2_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp2_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp2_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[2]);
	if (!hspi_send(usb_device_0.endpoints.rx[2].buffer, size, endpoint_mapping_reverse[2] & CUSTOM_REGISTER_ENDPOINT_MASK))
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "error in endp2_rx_callback, could not send to other board \r\n");
	}
	usb_device_0.endpoints.rx[2].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp3_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp3_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp3_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[3]);
	hspi_send(usb_device_0.endpoints.rx[3].buffer, size, endpoint_mapping_reverse[3] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[3].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp4_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp4_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp4_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[4]);
	hspi_send(usb_device_0.endpoints.rx[4].buffer, size, endpoint_mapping_reverse[4] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[4].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp5_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp5_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp5_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[5]);
	hspi_send(usb_device_0.endpoints.rx[5].buffer, size, endpoint_mapping_reverse[5] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[5].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp6_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp6_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp6_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[6]);
	hspi_send(usb_device_0.endpoints.rx[6].buffer, size, endpoint_mapping_reverse[6] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[6].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp7_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp7_rx_callback(uint8_t* const ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp7_rx_callback buffer %p size %d \r\n", ptr, size);
	hydradancer_status.ep_in_status &= ~(0x01 << endpoint_mapping_reverse[7]);
	hspi_send(usb_device_0.endpoints.rx[7].buffer, size, endpoint_mapping_reverse[7] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[7].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint16_t endp0_user_handled_control_request(USB_SETUP* request, uint8_t** buf);
uint16_t endp0_user_handled_control_request(USB_SETUP* request, uint8_t** buf)
{
	LOG_IF(LOG_LEVEL_TRACE, LOG_ID_USER, "endp0_user_handled_control_request bRequest %d  \r\n\r\n\r\n", request->bRequest);

	if (request->bRequest == GET_EP_STATUS)
	{
		*buf = (uint8_t*)&hydradancer_status;
		if (hydradancer_status.other_events != 0)
		{
			hydradancer_status.other_events = 0; // only send events once
		}
		return sizeof(hydradancer_status);
	}
	else if (request->bRequest == CHECK_HYDRADANCER_READY)
	{
		*buf = (uint8_t*)&boards_ready;
		return sizeof(boards_ready);
	}
	else if (request->bRequest == SET_ENDPOINT_MAPPING)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_ENDPOINT_MAPPING target_ep %d pc_ep %d \r\n", request->wValue.bw.bb1, request->wValue.bw.bb0);
		buf_0[0] = request->wValue.bw.bb1; // target endpoint
		buf_0[1] = request->wValue.bw.bb0; // pc endpoint
		endpoint_mapping_reverse[request->wValue.bw.bb0] = request->wValue.bw.bb1;
		hspi_send(buf_0, 2, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_SET_ENDPOINT_MAPPING << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		hydradancer_status.ep_in_status |= (0x01 << request->wValue.bw.bb1);
		hydradancer_status.ep_out_status &= ~(0x01 << request->wValue.bw.bb1);
		send_hydradancer_status();
		return 0;
	}
	else if (request->bRequest == SET_EP_RESPONSE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_EP_RESPONSE");
		buf_0[0] = request->wValue.bw.bb1;
		buf_0[1] = request->wValue.bw.bb0;
		hspi_send(buf_0, 2, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_SET_EP_RESPONSE << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}
	else if (request->bRequest == ENABLE_USB_CONNECTION_REQUEST_CODE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "ENABLE_USB_CONNECTION_REQUEST_CODE");
		hspi_send(buf_0, 0, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_ENABLE_USB_CONNECTION_REQUEST_CODE << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}
	else if (request->bRequest == SET_SPEED)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_SPEED");
		buf_0[0] = request->wValue.bw.bb1;
		hspi_send(buf_0, 1, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_SET_SPEED << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}
	else if (request->bRequest == DISABLE_USB)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "DISABLE_USB");
		boards_ready = 0;
		hspi_send(buf_0, 0, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_DISABLE_USB << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}
	else if (request->bRequest == DO_BUS_RESET)
	{
		hspi_send(buf_0, 0, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_EMULATION_BOARD_BUS_RESET << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}
	else if (request->bRequest == SET_ADDRESS_REQUEST_CODE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_ADDRESS_REQUEST_CODE");
		buf_0[0] = request->wValue.bw.bb1;
		hspi_send(buf_0, 1, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_SET_ADDRESS_REQUEST_CODE << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
		return 0;
	}

	return 0xffff;
}

#endif
