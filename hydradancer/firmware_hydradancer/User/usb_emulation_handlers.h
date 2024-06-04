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
	hydradancer_status_clear_nak(endp_num);
	hydradancer_status_set_in(endp_num);
	write_event(EVENT_IN_BUFFER_AVAILABLE, endp_num);
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
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;

	// we need to wait here, because we can't NAK a packet with PID setup
	// the only accepted answer is ACK, so we have to store it and wait for Facedancer to be ready
	uint64_t start = bsp_get_SysTickCNT();
	uint64_t curr = 0;
	uint64_t delta = 0;

	while (hydradancer_status.ep_out_status & (0x01 << 0) && delta < MAX_BUSY_WAIT_CYCLES)
	{
		if (start_polling)
			hydradancer_send_event();
		WWDG_SetCounter(0); // rearm the watchdog
		curr = bsp_get_SysTickCNT();
		delta = start - curr; // SysTickCNT is decremented so comparison is inverted
	}

	hydradancer_status_set_out(0);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	write_event(EVENT_OUT_BUFFER_AVAILABLE, 0);

	// wait for the packet to be transmitted or count to reach MAX_COUNT
	// we still miss some interrupts, count attemps to mitigate this issue by considering
	// the packet has been sent after some time
	start = bsp_get_SysTickCNT();
	curr = 0;
	delta = 0;
	while (hydradancer_status.ep_out_status & (0x01 << 0) && delta < MAX_BUSY_WAIT_CYCLES)
	{
		if (start_polling)
			hydradancer_send_event();
		WWDG_SetCounter(0); // rearm the watchdog
		curr = bsp_get_SysTickCNT();
		delta = start - curr; // SysTickCNT is decremented so comparison is inverted
	}

	if (delta >= MAX_BUSY_WAIT_CYCLES)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "_usb_emulation_endp0_passthrough_setup_callback RECOVERY\r\n");
		hydradancer_recover_out_interrupt(0);
	}
	return true;
}

uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size);
uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size)
{
	uint8_t* buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	if (buffer == NULL)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp0_passthrough_setup_callback Could not allocate pool buf\r\n");
		return ENDP_STATE_NAK;
	}
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	if (ep_queue_member == NULL)
	{
		ramx_pool_free(buffer);
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp0_passthrough_setup_callback could not allocate ep_queue_member");
		return ENDP_STATE_NAK;
	}
	memcpy(buffer, ptr, size);
	ep_queue_member->ptr = buffer;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_passthrough_setup_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	return ENDP_STATE_NAK;
}

static bool _usb_emulation_endp_rx_callback(uint8_t* data)
{
	ep_queue_member_t* ep_queue_member = (ep_queue_member_t*)data;
	hydradancer_status_set_out(ep_queue_member->ep_num);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[ep_queue_member->ep_num], ep_queue_member->ptr, ep_queue_member->size);
	write_event(EVENT_OUT_BUFFER_AVAILABLE, ep_queue_member->ep_num);

	uint64_t start = bsp_get_SysTickCNT();
	uint64_t curr = 0;
	uint64_t delta = 0;

	while (hydradancer_status.ep_out_status & (0x01 << ep_queue_member->ep_num) && delta < MAX_BUSY_WAIT_CYCLES)
	{
		if (start_polling)
			hydradancer_send_event();
		WWDG_SetCounter(0); // rearm the watchdog
		curr = bsp_get_SysTickCNT();
		delta = start - curr; // SysTickCNT is decremented so comparison is inverted
	}

	if (delta >= MAX_BUSY_WAIT_CYCLES)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "_usb_emulation_endp_rx_callback RECOVERY\r\n");
		hydradancer_recover_out_interrupt(ep_queue_member->ep_num);
	}
	return true;
}

__attribute__((always_inline)) static inline uint8_t usb_emulation_endp_rx_callback(uint8_t* const ptr, uint16_t size, uint8_t endp_num)
{
	if (hydradancer_status.ep_out_status & (0x01 << endp_num))
	{
		return ENDP_STATE_NAK;
	}

	uint8_t* buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	if (buffer == NULL)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp_rx_callback could not allocate buffer\r\n");
		return ENDP_STATE_NAK;
	}
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	if (ep_queue_member == NULL)
	{
		ramx_pool_free(buffer);
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp_rx_callback could not allocate ep queue member\r\n");
		return ENDP_STATE_NAK;
	}

	ep_queue_member->ep_num = endp_num;
	ep_queue_member->ptr = ptr;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	usb_device_0.endpoints.rx[endp_num].buffer = buffer;
	return ENDP_STATE_NAK;
}

bool _usb_emulation_endp0_rx_callback(uint8_t* data);
bool _usb_emulation_endp0_rx_callback(uint8_t* data)
{
	hydradancer_status_set_out(0);

	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;

	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	write_event(EVENT_OUT_BUFFER_AVAILABLE, 0);

	uint64_t start = bsp_get_SysTickCNT();
	uint64_t curr = 0;
	uint64_t delta = 0;

	while (hydradancer_status.ep_out_status & (0x01 << 0) && delta < MAX_BUSY_WAIT_CYCLES)
	{
		if (start_polling)
			hydradancer_send_event();
		WWDG_SetCounter(0); // rearm the watchdog
		curr = bsp_get_SysTickCNT();
		delta = start - curr; // SysTickCNT is decremented so comparison is inverted
	}

	if (delta >= MAX_BUSY_WAIT_CYCLES)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "_usb_emulation_endp0_rx_callback RECOVERY\r\n");
		hydradancer_recover_out_interrupt(0);
	}
	return true;
}

uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size)
{
	if (hydradancer_status.ep_out_status & (0x01 << 0))
	{
		return ENDP_STATE_NAK;
	}

	uint8_t* buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	if (buffer == NULL)
	{
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp0_passthrough_setup_callback Could not allocate pool buf\r\n");
		return ENDP_STATE_NAK;
	}
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	if (ep_queue_member == NULL)
	{
		ramx_pool_free(buffer);
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_emulation_endp0_passthrough_setup_callback could not allocate ep_queue_member");
		return ENDP_STATE_NAK;
	}
	memcpy(buffer, ptr, size);
	ep_queue_member->ptr = buffer;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	return ENDP_STATE_NAK;
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
		hydradancer_status_set_nak(endp_num);
		write_event(EVENT_NAK, endp_num);
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

	write_event(EVENT_BUS_RESET, 0);
}

#endif
