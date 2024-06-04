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
#include "wch-ch56x-lib/memory/fifo.h"
#include "wch-ch56x-lib/memory/pool.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb2_utils.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

__attribute__((always_inline)) static inline void usb_emulation_endp_tx_complete(TRANSACTION_STATUS status, uint8_t endp_num)
{
	ramx_pool_free(usb_device_0.endpoints.tx[endp_num].buffer);
	hydradancer_status.ep_in_nak &= ~(0x01 << endp_num);
	hydradancer_status.ep_in_status |= (0x01 << endp_num);
	hydradancer_event_t event = {
		.type = EVENT_IN_BUFFER_AVAILABLE,
		.value = endp_num,
	};
	fifo_write(&event_queue, &event, 1);
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
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp0_passthrough_setup_callback size %d data %d %d %d %d %d %d \r\n", size, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;
	hydradancer_status.ep_out_status |= (0x01 << 0);
	bsp_disable_interrupt();
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	bsp_enable_interrupt();
	hydradancer_event_t event = {
		.type = EVENT_OUT_BUFFER_AVAILABLE,
		.value = 0,
	};
	fifo_write(&event_queue, &event, 1);
	return true;
}

uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size);
uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	ep_queue_member->ptr = ptr;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_passthrough_setup_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup, INTERRUPT_QUEUE_LOW_PRIO);
	return ENDP_STATE_ACK;
}

static bool _usb_emulation_endp_rx_callback(uint8_t* data)
{
	ep_queue_member_t* ep_queue_member = (ep_queue_member_t*)data;
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[ep_queue_member->ep_num], ep_queue_member->ptr, ep_queue_member->size);
	hydradancer_event_t event = {
		.type = EVENT_OUT_BUFFER_AVAILABLE,
		.value = ep_queue_member->ep_num,
	};
	fifo_write(&event_queue, &event, 1);
	return ENDP_STATE_ACK;
}

__attribute__((always_inline)) static inline uint8_t usb_emulation_endp_rx_callback(uint8_t* const ptr, uint16_t size, uint8_t endp_num)
{
	if (hydradancer_status.ep_out_status & (0x01 << endp_num))
	{
		return ENDP_STATE_NAK;
	}
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp%d_rx_callback %d %d %d %d %d %d \r\n", endp_num, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	ep_queue_member->ep_num = endp_num;
	ep_queue_member->ptr = ptr;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup, INTERRUPT_QUEUE_LOW_PRIO);
	hydradancer_status.ep_out_status |= (0x01 << endp_num);
	usb_device_0.endpoints.rx[endp_num].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
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
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "endp%d_rx_callback size %d data %d %d %d %d %d %d \r\n", 0, size, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;
	hydradancer_status.ep_out_status |= (0x01 << 0);
	bsp_disable_interrupt();
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	bsp_enable_interrupt();
	hydradancer_event_t event = {
		.type = EVENT_OUT_BUFFER_AVAILABLE,
		.value = 0,
	};
	fifo_write(&event_queue, &event, 1);
	return true;
}

uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	ep_queue_member->ptr = ptr;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup, INTERRUPT_QUEUE_LOW_PRIO);
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

void usb_emulation_nak_callback(uint8_t endp_num);
void usb_emulation_nak_callback(uint8_t endp_num)
{
	if (!(hydradancer_status.ep_in_nak & (0x01 << endp_num)))
	{
		hydradancer_status.ep_in_nak |= (0x01 << endp_num);
		hydradancer_event_t event = {
			.type = EVENT_NAK,
			.value = endp_num,
		};
		fifo_write(&event_queue, &event, 1);
	}
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
	bsp_disable_interrupt();
	hydradancer_status.ep_in_status = (0x1 << 0) & 0xff; // keep ep0
	hydradancer_status.ep_out_status = 0;
	hydradancer_status.ep_in_nak = 0;
	hydradancer_status.other_events = 0;

	// Set the USB device parameters
	usb2_ep0_passthrough_enabled(true);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);

	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}

	boards_ready = 1;
	bsp_enable_interrupt();

	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "bus reset \r\n");
	hydradancer_event_t event = {
		.type = EVENT_BUS_RESET,
		.value = 0,
	};
	fifo_write(&event_queue, &event, 1);
}

#endif
