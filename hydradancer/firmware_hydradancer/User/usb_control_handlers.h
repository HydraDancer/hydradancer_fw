#ifndef USB_CONTROL_HANDLERS_H
#define USB_CONTROL_HANDLERS_H

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
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb2_utils.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb30_utils.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"
#include "wch-ch56x-lib/USBDevice/usb_types.h"

__attribute__((always_inline)) static inline void usb_control_endp1_tx_complete(TRANSACTION_STATUS status)
{
	hydradancer_set_event_transfer_finished(true);
}

__attribute__((always_inline)) static inline void usb_control_endp_tx_complete(TRANSACTION_STATUS status, uint8_t endp_num)
{
	if (hydradancer_status.ep_out_status & (0x01 << endpoint_mapping_reverse[endp_num]))
	{
		if (endpoint_mapping_reverse[endp_num] == 0)
			ramx_pool_free(usb_device_1.endpoints.tx[endp_num].buffer);
		hydradancer_status_clear_out(endpoint_mapping_reverse[endp_num]);
		endp_rx_set_state(&usb_device_0, endpoint_mapping_reverse[endp_num], ENDP_STATE_ACK);
	}
}

void usb_control_endp2_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp2_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 2);
}

void usb_control_endp3_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp3_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 3);
}

void usb_control_endp4_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp4_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 4);
}
void usb_control_endp5_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp5_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 5);
}

void usb_control_endp6_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp6_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 6);
}

void usb_control_endp7_tx_complete(TRANSACTION_STATUS status);
void usb_control_endp7_tx_complete(TRANSACTION_STATUS status)
{
	usb_control_endp_tx_complete(status, 7);
}

bool _usb0_ep0_send(uint8_t* data);
bool _usb0_ep0_send(uint8_t* data)
{
	uint8_t* ptr = ((ep_queue_member_t*)data)->ptr;
	uint16_t size = ((ep_queue_member_t*)data)->size;
	endp_tx_set_new_buffer(&usb_device_0, endpoint_mapping_reverse[0], ptr, size);
	return true;
}

static bool _usb_control_endp_rx_callback(uint8_t* data);
static bool _usb_control_endp_rx_callback(uint8_t* data)
{
	ep_queue_member_t* ep_queue_member = (ep_queue_member_t*)data;
	while (true)
	{
		BSP_ENTER_CRITICAL();
		volatile uint16_t status = hydradancer_status.ep_in_status;
		BSP_EXIT_CRITICAL();
		if (status & (0x01 << endpoint_mapping_reverse[ep_queue_member->ep_num]))
			break;
	}
	endp_tx_set_new_buffer(&usb_device_0, endpoint_mapping_reverse[ep_queue_member->ep_num], ep_queue_member->ptr, ep_queue_member->size);
	hydradancer_status_clear_in(endpoint_mapping_reverse[ep_queue_member->ep_num]);
	return true;
}

__attribute__((always_inline)) static inline uint8_t usb_control_endp_rx_callback(uint8_t* const ptr, uint16_t size, uint8_t endp_num)
{
	if (!(hydradancer_status.ep_in_status & (0x01 << endpoint_mapping_reverse[endp_num])))
	{
		return ENDP_STATE_NAK;
	}

	if (endpoint_mapping_reverse[endp_num] == 0)
	{
		ep_queue_member_t* ep_queue_member = hydra_pool_get(&ep_queue);
		if (ep_queue_member == NULL)
		{
			LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "usb_control_endp_rx_callback could not allocate pool member");
			return ENDP_STATE_NAK;
		}
		ep_queue_member->ep_num = endp_num;
		ep_queue_member->ptr = ptr;
		ep_queue_member->size = size;
		hydra_interrupt_queue_set_next_task(_usb_control_endp_rx_callback, (uint8_t*)ep_queue_member, _ep_queue_cleanup);
	}
	else
	{
		hydradancer_status_clear_in(endpoint_mapping_reverse[endp_num]);
		endp_tx_set_new_buffer(&usb_device_0, endpoint_mapping_reverse[endp_num], ptr, size);
	}
	return ENDP_STATE_ACK;
}

uint8_t usb_control_endp2_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp2_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 2);
}

uint8_t usb_control_endp3_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp3_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 3);
}

uint8_t usb_control_endp4_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp4_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 4);
}

uint8_t usb_control_endp5_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp5_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 5);
}

uint8_t usb_control_endp6_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp6_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 6);
}

uint8_t usb_control_endp7_rx_callback(uint8_t* const ptr, uint16_t size);
uint8_t usb_control_endp7_rx_callback(uint8_t* const ptr, uint16_t size)
{
	return usb_control_endp_rx_callback(ptr, size, 7);
}

bool do_disable_usb(void);
bool do_disable_usb(void)
{
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "DISABLE_USB\r\n");
	BSP_ENTER_CRITICAL();
	usb2_device_deinit();
	boards_ready = 0;
	start_polling = false;
	hydradancer_set_event_transfer_finished(true);
	hydra_interrupt_queue_free_all();
	hydra_interrupt_queue_init();
	hydra_pool_clean(&ep_queue);
	fifo_clean(&event_queue);
	ramx_pool_init();
	usb_emulation_reinit();
	usb_control_reinit();
	BSP_EXIT_CRITICAL();
	return true;
}

uint16_t usb_control_endp0_user_handled_control_request(USB_SETUP* request, uint8_t** buf);
uint16_t usb_control_endp0_user_handled_control_request(USB_SETUP* request, uint8_t** buf)
{
	LOG_IF(LOG_LEVEL_TRACE, LOG_ID_USER, "endp0_user_handled_control_request bRequest %d  \r\n", request->bRequest);

	if (request->bRequest == GET_EVENT)
	{
		*buf = (uint8_t*)_events_buffer;
		uint16_t events_count = fifo_count(&event_queue);
		if (events_count > 0)
		{
			uint16_t count_read = fifo_read_n(&event_queue, (void*)_events_buffer, events_count);
			return count_read * sizeof(hydradancer_event_t);
		}
		return 0;
	}
	else if (request->bRequest == CLEAR_HALT)
	{
		LOG("CLEAR_HALT ep %d dir %d \r\n", request->wValue.bw.bb1, request->wValue.bw.bb0);

		uint32_t ep = 0;
		if (request->wValue.bw.bb0 == 0)
		{
			ep = 1U << ((request->wValue.bw.bb1 - 1) * 2 + 1);
			usb_device_0.endpoints.rx[request->wValue.bw.bb1].state = ENDP_STATE_ACK;
		}
		else
		{
			ep = 1U << ((request->wValue.bw.bb1 - 1) * 2);
			usb_device_0.endpoints.tx[request->wValue.bw.bb1].state = ENDP_STATE_NAK;
			write_event(EVENT_IN_BUFFER_AVAILABLE, request->wValue.bw.bb1);
		}
		usb2_setup_endpoints_in_mask(ep);
		return 0;
	}
	else if (request->bRequest == CHECK_HYDRADANCER_READY)
	{
		*buf = (uint8_t*)&boards_ready;
		return sizeof(boards_ready);
	}
	else if (request->bRequest == SET_ENDPOINT_MAPPING)
	{
		LOG("SET_ENDPOINT_MAPPING target_ep %d pc_ep %d \r\n", request->wValue.bw.bb1, request->wValue.bw.bb0);
		endpoint_mapping[request->wValue.bw.bb1] = request->wValue.bw.bb0;
		endpoint_mapping_reverse[request->wValue.bw.bb0] = request->wValue.bw.bb1;
		hydradancer_status.ep_in_status |= (0x01 << request->wValue.bw.bb1);
		hydradancer_status.ep_out_status &= ~(0x01 << request->wValue.bw.bb1);
		usb_device_0.endpoints.rx[request->wValue.bw.bb1].max_packet_size = usb_device_0.speed == USB2_HIGHSPEED ? 512 : 64;
		usb_device_0.endpoints.rx[request->wValue.bw.bb1].state = ENDP_STATE_ACK;
		usb_device_0.endpoints.tx[request->wValue.bw.bb1].state = ENDP_STATE_NAK;

		if (request->wValue.bw.bb1 > 0)
		{
			uint32_t new_eps = ((1U << ((request->wValue.bw.bb1 - 1) * 2)) | (1U << (((request->wValue.bw.bb1 - 1) * 2 + 1))));
			usb_device_0.endpoint_mask |= new_eps;
			usb2_setup_endpoints_in_mask(new_eps);
		}
		write_event(EVENT_IN_BUFFER_AVAILABLE, request->wValue.bw.bb1);
		return 0;
	}
	else if (request->bRequest == SET_EP_RESPONSE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_EP_RESPONSE\r\n");
		if (request->wValue.bw.bb1 & 0x80)
		{
			endp_tx_set_state(&usb_device_0, request->wValue.bw.bb1 & 0x7f, request->wValue.bw.bb0);
		}
		else
		{
			endp_rx_set_state(&usb_device_0, request->wValue.bw.bb1 & 0x7f, request->wValue.bw.bb0);
		}
		return 0;
	}
	else if (request->bRequest == ENABLE_USB_CONNECTION_REQUEST_CODE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "ENABLE_USB_CONNECTION_REQUEST_CODE\r\n");
		usb2_device_init();
		usb2_enable_nak(true);
		return 0;
	}
	else if (request->bRequest == SET_SPEED)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_SPEED\r\n");
		switch (request->wValue.bw.bb1)
		{
		case USB2_LS:
			usb_device_0.speed = USB2_LOWSPEED;
			break;
		case USB2_FS:
			usb_device_0.speed = USB2_FULLSPEED;
			break;
		case USB2_HS:
			usb_device_0.speed = USB2_HIGHSPEED;
			break;
		default:
			usb_device_0.speed = USB2_FULLSPEED;
			break;
		}
		return 0;
	}
	else if (request->bRequest == DISABLE_USB)
	{
		do_disable_usb();
		return 0;
	}
	else if (request->bRequest == SET_ADDRESS_REQUEST_CODE)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SET_ADDRESS_REQUEST_CODE\r\n");
		usb2_set_device_address(request->wValue.bw.bb1);
		return 0;
	}
	else if (request->bRequest == CONFIGURED)
	{
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "CONFIGURED\r\n");
		start_polling = true;
		return 0;
	}

	return 0xffff;
}

#endif
