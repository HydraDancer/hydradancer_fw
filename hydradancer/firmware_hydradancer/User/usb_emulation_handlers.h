#ifndef USB_EMULATION_HANDLERS
#define USB_EMULATION_HANDLERS

// Disable warnings in bsp arising from -pedantic -Wall -Wconversion
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "CH56x_common.h"
#include "CH56xSFR.h"
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

#include "definitions.h"
#include "usb_emulation_device.h"
#include "wch-ch56x-lib/interrupt_queue/interrupt_queue.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/pool.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb2_utils.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

__attribute__((always_inline)) static inline void usb_emulation_endp_tx_complete(TRANSACTION_STATUS status, uint8_t endp_num)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp%d_tx_complete\r\n", endp_num);
	ramx_pool_free(usb_device_0.endpoints.tx[endp_num].buffer);
	hydradancer_status.ep_in_status |= (0x01 << endp_num);
	send_hydradancer_status();
}

void usb_emulation_endp0_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp0_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 0);
}

void usb_emulation_endp1_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp1_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 1);
}

void usb_emulation_endp2_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp2_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 2);
}

void usb_emulation_endp3_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp3_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 3);
}

void usb_emulation_endp4_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp4_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 4);
}

void usb_emulation_endp5_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp5_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 5);
}

void usb_emulation_endp6_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp6_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 6);
}

void usb_emulation_endp7_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp7_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 7);
}

bool _usb_emulation_endp0_passthrough_setup_callback(uint8_t* data);
bool _usb_emulation_endp0_passthrough_setup_callback(uint8_t* data)
{
	// can't NAK a setup, can't STALL because it would mean the request is not supported
	// so we just wait
	while (hydradancer_status.ep_out_status & (0x01 << 0))
	{
		WWDG_SetCounter(0); // rearm the watchdog
		continue;
	}
	uint8_t* ptr = ((ep0_queue_member_t*)data)->ptr;
	uint16_t size = ((ep0_queue_member_t*)data)->size;
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp0_passthrough_setup_callback size %d data %d %d %d %d %d %d \r\n", size, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	hydradancer_status.ep_out_status |= (0x01 << 0);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	send_hydradancer_status();
	return true;
}

uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size);
uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ep0_queue_member_t* ep0_queue_member = hydra_pool_get(&ep0_queue);
	ep0_queue_member->ptr = ptr;
	ep0_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_passthrough_setup_callback, (uint8_t*)ep0_queue_member, _ep0_queue_cleanup, INTERRUPT_QUEUE_LOW_PRIO);
	return ENDP_STATE_ACK;
}

__attribute__((always_inline)) static inline uint8_t usb_emulation_endp_rx_callback(uint8_t* const ptr, uint16_t size, uint8_t endp_num)
{
	if (hydradancer_status.ep_out_status & (0x01 << endp_num))
	{
		return ENDP_STATE_NAK;
	}
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp%d_rx_callback %d %d %d %d %d %d \r\n", endp_num, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	hydradancer_status.ep_out_status |= (0x01 << endp_num);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[endp_num], ptr, size);
	usb_device_0.endpoints.rx[endp_num].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	send_hydradancer_status();
	return ENDP_STATE_ACK;
}

bool _usb_emulation_endp0_rx_callback(uint8_t* data);
bool _usb_emulation_endp0_rx_callback(uint8_t* data)
{
	while (hydradancer_status.ep_out_status & (0x01 << 0))
	{
		WWDG_SetCounter(0); // rearm the watchdog
		continue;
	}
	uint8_t* ptr = ((ep0_queue_member_t*)data)->ptr;
	uint16_t size = ((ep0_queue_member_t*)data)->size;
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp%d_rx_callback size %d data %d %d %d %d %d %d \r\n", 0, size, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	hydradancer_status.ep_out_status |= (0x01 << 0);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	send_hydradancer_status();
	return true;
}

uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ep0_queue_member_t* ep0_queue_member = hydra_pool_get(&ep0_queue);
	ep0_queue_member->ptr = ptr;
	ep0_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_rx_callback, (uint8_t*)ep0_queue_member, _ep0_queue_cleanup, INTERRUPT_QUEUE_LOW_PRIO);
	return ENDP_STATE_ACK;
}

uint8_t usb_emulation_endp1_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp1_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 1);
}

uint8_t usb_emulation_endp2_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp2_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 2);
}
uint8_t usb_emulation_endp3_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp3_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 3);
}
uint8_t usb_emulation_endp4_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp4_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 4);
}
uint8_t usb_emulation_endp5_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp5_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 5);
}

uint8_t usb_emulation_endp6_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp6_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 6);
}

uint8_t usb_emulation_endp7_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp7_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 7);
}

/*
1. Warn Facedancer about the bus reset via the control board
2. Facedancer should then trigger a usb vendor control reset to the control board
3. the control board tells the emulation board to handle it's side of the bus reset
4. the emulation board tells the control board to reset as well
*/
void usb_emulation_usb2_device_handle_bus_reset(void);
void usb_emulation_usb2_device_handle_bus_reset(void)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "bus reset \r\n");
	hydradancer_status.other_events |= HYDRADANCER_STATUS_BUS_RESET;
	send_hydradancer_status();
}

#endif
