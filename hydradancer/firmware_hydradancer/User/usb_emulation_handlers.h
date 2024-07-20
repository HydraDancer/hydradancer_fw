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
#include "wch-ch56x-lib/USBDevice/usb30_utils.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"
#include "wch-ch56x-lib/USBDevice/usb_types.h"

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

void usb_emulation_endp8_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp8_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 8);
}

void usb_emulation_endp9_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp9_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 9);
}

void usb_emulation_endp10_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp10_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 10);
}

void usb_emulation_endp11_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp11_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 11);
}

void usb_emulation_endp12_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp12_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 12);
}

void usb_emulation_endp13_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp13_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 13);
}

void usb_emulation_endp14_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp14_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 14);
}

void usb_emulation_endp15_tx_complete(TRANSACTION_STATUS status);
void usb_emulation_endp15_tx_complete(TRANSACTION_STATUS status)
{
	usb_emulation_endp_tx_complete(status, 15);
}

bool _usb_emulation_endp0_passthrough_setup_callback(uint8_t* data);
bool _usb_emulation_endp0_passthrough_setup_callback(uint8_t* data)
{
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;

	while (true)
	{
		bsp_disable_interrupt();
		volatile uint16_t status = hydradancer_status.ep_out_status;
		bsp_enable_interrupt();
		if (!(status & (0x01 << 0)))
			break;
		if (start_polling)
			hydradancer_send_event();
	}

	hydradancer_status_set_out(0);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[0], ptr, size);
	write_event(EVENT_OUT_BUFFER_AVAILABLE, 0);

	while (true)
	{
		bsp_disable_interrupt();
		volatile uint16_t status = hydradancer_status.ep_out_status;
		bsp_enable_interrupt();
		if (!(status & (0x01 << 0)))
			break;
		if (start_polling)
			hydradancer_send_event();
	}

	return true;
}

uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size);
uint8_t usb_emulation_endp0_passthrough_setup_callback(uint8_t* ptr, uint16_t size)
{
	uint8_t* buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	memcpy(buffer, ptr, size);
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	ep_queue_member->ptr = buffer;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp0_passthrough_setup_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	return ENDP_STATE_NAK;
}

static bool _usb_emulation_endp_rx_callback(uint8_t* data)
{
	uint8_t endp_num = ((ep_queue_member_t*)data)->ep_num;
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;

	hydradancer_status_set_out(endp_num);
	endp_tx_set_new_buffer(&usb_device_1, endpoint_mapping[endp_num], ptr, size);
	write_event(EVENT_OUT_BUFFER_AVAILABLE, endp_num);

	while (true)
	{
		bsp_disable_interrupt();
		volatile uint16_t status = hydradancer_status.ep_out_status;
		bsp_enable_interrupt();
		if (!(status & (0x01 << endp_num)))
		{
			break;
		}

		if (start_polling)
			hydradancer_send_event();
	}
	return true;
}

__attribute__((always_inline)) static inline uint8_t usb_emulation_endp_rx_callback(uint8_t* const ptr, uint16_t size, uint8_t endp_num)
{
	uint8_t* buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);

	if (endp_num == 0)
	{
		memcpy(buffer, ptr, size);
	}
	else
	{
		usb_device_0.endpoints.rx[endp_num].buffer = buffer;
	}
	ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
	ep_queue_member->ep_num = endp_num;
	ep_queue_member->ptr = endp_num == 0 ? buffer : ptr;
	ep_queue_member->size = size;
	hydra_interrupt_queue_set_next_task(_usb_emulation_endp_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	return ENDP_STATE_NAK;
}

void usb2_out_nak(uint8_t endp);
void usb2_out_nak(uint8_t endp)
{
}

uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp0_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 0);
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

uint8_t usb_emulation_endp8_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp8_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 8);
}

uint8_t usb_emulation_endp9_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp9_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 9);
}

uint8_t usb_emulation_endp10_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp10_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 10);
}

uint8_t usb_emulation_endp11_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp11_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 11);
}

uint8_t usb_emulation_endp12_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp12_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 12);
}

uint8_t usb_emulation_endp13_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp13_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 13);
}

uint8_t usb_emulation_endp14_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp14_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 14);
}

uint8_t usb_emulation_endp15_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_emulation_endp15_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_emulation_endp_rx_callback(ptr, size, 15);
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

	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}

	boards_ready = 1;
	bsp_enable_interrupt();

	write_event(EVENT_BUS_RESET, 0);
}

#endif
