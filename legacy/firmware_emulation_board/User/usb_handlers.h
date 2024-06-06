#ifndef USB_HANDLERS_H
#define USB_HANDLERS_H

#include "definitions.h"
#include "usb_device.h"
#include "wch-ch56x-lib/HSPIDeviceScheduled/hspi_scheduled.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb2_utils.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

void endp0_tx_complete(TRANSACTION_STATUS status);
void endp0_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp0_tx_complete\r\n");
	hydradancer_status.ep_in_status |= (0x01 << 0);
	ep_status_buffer[0] = 0;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp1_tx_complete(TRANSACTION_STATUS status);
void endp1_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp1_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[1].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 1);
	ep_status_buffer[0] = 1;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp2_tx_complete(TRANSACTION_STATUS status);
void endp2_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp2_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[2].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 2);
	ep_status_buffer[0] = 2;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp3_tx_complete(TRANSACTION_STATUS status);
void endp3_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp3_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[3].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 3);
	ep_status_buffer[0] = 3;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp4_tx_complete(TRANSACTION_STATUS status);
void endp4_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp4_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[4].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 4);
	ep_status_buffer[0] = 4;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp5_tx_complete(TRANSACTION_STATUS status);
void endp5_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp5_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[5].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 5);
	ep_status_buffer[0] = 5;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp6_tx_complete(TRANSACTION_STATUS status);
void endp6_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp6_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[6].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 6);
	ep_status_buffer[0] = 6;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

void endp7_tx_complete(TRANSACTION_STATUS status);
void endp7_tx_complete(TRANSACTION_STATUS status)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp7_tx_complete\r\n");
	ramx_pool_free(usb_device_0.endpoints.tx[7].buffer);
	hydradancer_status.ep_in_status |= (0x01 << 7);
	ep_status_buffer[0] = 7;
	ep_status_buffer[1] = 1;
	hspi_send(ep_status_buffer, sizeof(ep_status_buffer), (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_UPDATE_EP_IN_STATUS << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

uint8_t endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size);
uint8_t endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp0_passthrough_setup_callback %d %d %d %d %d %d \r\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	hydradancer_status.ep_out_status |= (0x01 << 0);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[0] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_NAK;
}

uint8_t endp0_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp0_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 0))
	{
		return ENDP_STATE_NAK;
	}
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp0_rx_callback %d %d %d %d %d %d \r\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	hydradancer_status.ep_out_status |= (0x01 << 0);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[0] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp1_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp1_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 1))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 1);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[1] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[1].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp2_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp2_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 2))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 2);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[2] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[2].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}
uint8_t endp3_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp3_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 3))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 3);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[3] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[3].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}
uint8_t endp4_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp4_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 4))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 4);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[4] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[4].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}
uint8_t endp5_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp5_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 5))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 5);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[5] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[5].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp6_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp6_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 6))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 6);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[6] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[6].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

uint8_t endp7_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t endp7_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 7))
	{
		return ENDP_STATE_NAK;
	}
	hydradancer_status.ep_out_status |= (0x01 << 7);
	hspi_send((uint8_t*)ptr, size, endpoint_mapping[7] & CUSTOM_REGISTER_ENDPOINT_MASK);
	usb_device_0.endpoints.rx[7].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ramx_pool_free((uint8_t*)ptr);
	return ENDP_STATE_ACK;
}

/*
1. Warn Facedancer about the bus reset via the control board
2. Facedancer should then trigger a usb vendor control reset to the control board
3. the control board tells the emulation board to handle it's side of the bus reset
4. the emulation board tells the control board to reset as well
*/
void usb2_device_handle_bus_reset(void);
void usb2_device_handle_bus_reset(void)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "bus reset \r\n");
	hspi_send(&placeholder, 1, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_EMULATION_BOARD_BUS_RESET << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
}

#endif