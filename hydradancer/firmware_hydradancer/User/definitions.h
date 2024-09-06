#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define ENDP_1_15_MAX_PACKET_SIZE 1024

/* Global define */
// DEF_ENDP_OUT_BURST_LEVEL / DEF_ENDP_IN_BURST_LEVEL maximum burst size 16 defined by the USB3 specification
// Warning USB3 endpoint bulk with 8 or 16 burst can be problematic on some PC
#define DEF_ENDP_OUT_BURST_LEVEL 1
#define DEF_ENDP_IN_BURST_LEVEL (DEF_ENDP_OUT_BURST_LEVEL)
#define DEF_ENDP_MAX_SIZE (DEF_ENDP1_OUT_BURST_LEVEL * ENDP_1_15_MAX_PACKET_SIZE)

#include "wch-ch56x-lib/memory/fifo.h"
#include "wch-ch56x-lib/memory/pool.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb_device.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

#define USB2_LS 0x00
#define USB2_FS 0x01
#define USB2_HS 0x02

// Standard control requests codes go from 0-27 and 48-49
#define ENABLE_USB_CONNECTION_REQUEST_CODE 50
#define SET_ADDRESS_REQUEST_CODE 51
#define GET_EVENT 52
#define SET_ENDPOINT_MAPPING 53
#define DISABLE_USB 54
#define SET_SPEED 55
#define SET_EP_RESPONSE 56
#define CHECK_HYDRADANCER_READY 57
#define DO_BUS_RESET 58
#define CONFIGURED 59

#define EVENT_BUS_RESET 0x0
#define EVENT_IN_BUFFER_AVAILABLE 0x1
#define EVENT_OUT_BUFFER_AVAILABLE 0x2
#define EVENT_NAK 0x3

#undef FREQ_SYS
/* System clock / MCU frequency in Hz (lowest possible speed 15MHz) */
#define FREQ_SYS (120000000)

/* Blink time in ms */
#define BLINK_FAST (50) // Blink LED each 100ms (50*2)
#define BLINK_USB3 (250) // Blink LED each 500ms (250*2)
#define BLINK_USB2 (1000) // Blink LED each 2000ms (1000*2)

typedef struct __attribute__((packed))
{
	uint16_t ep_in_status;
	uint16_t ep_out_status;
	uint16_t ep_in_nak;
	uint16_t other_events;
} hydradancer_status_t;

typedef struct __attribute__((packed))
{
	uint8_t type;
	uint8_t value;
} hydradancer_event_t;

#define MAX_ENDPOINTS_SUPPORTED 16 //including EP0
extern uint8_t endpoint_mapping_reverse[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping_reverse[PC_Endpoint] = Target_Endpoint
extern uint8_t endpoint_mapping[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping[Target_Endpoint] = PC_Endpoint
extern __attribute__((aligned(16))) volatile hydradancer_status_t hydradancer_status __attribute__((section(".DMADATA")));
extern __attribute__((aligned(16))) volatile uint8_t boards_ready __attribute__((section(".DMADATA")));
extern __attribute__((aligned(16))) volatile bool event_transfer_finished;
extern __attribute__((aligned(16))) volatile bool start_polling;

#define EVENT_QUEUE_SIZE 100 // don't forget to update it in the Facedancer backend as well

extern __attribute__((aligned(16))) volatile hydradancer_event_t _events_buffer[EVENT_QUEUE_SIZE] __attribute__((section(".DMADATA")));

typedef struct __attribute__((packed))
{
	uint8_t ep_num;
	uint8_t* ptr;
	uint16_t size;
} ep_queue_member_t;

HYDRA_POOL_DECLR(ep_queue);

void _ep_queue_cleanup(uint8_t* data);

HYDRA_FIFO_DECLR(event_queue, hydradancer_event_t, EVENT_QUEUE_SIZE);

void hydradancer_send_event(void);

__attribute__((always_inline)) inline static void write_event(uint8_t type, uint8_t value)
{
	BSP_ENTER_CRITICAL();
	hydradancer_event_t event = {
		.type = type,
		.value = value,
	};
	fifo_write(&event_queue, &event, 1);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static bool hydradancer_get_event_transfer_finished()
{
	bool ret = false;
	BSP_ENTER_CRITICAL();
	ret = event_transfer_finished;
	BSP_EXIT_CRITICAL();
	return ret;
}

__attribute__((always_inline)) inline static void hydradancer_set_event_transfer_finished(bool _event_transfer_finished)
{
	BSP_ENTER_CRITICAL();
	event_transfer_finished = _event_transfer_finished;
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_set_out(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_out_status |= (1 << endp_num);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_set_in(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_in_status |= (1 << endp_num);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_set_nak(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_in_nak |= (1 << endp_num);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_clear_out(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_out_status &= ~(1 << endp_num);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_clear_in(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_in_status &= ~(1 << endp_num);
	BSP_EXIT_CRITICAL();
}

__attribute__((always_inline)) inline static void hydradancer_status_clear_nak(uint8_t endp_num)
{
	BSP_ENTER_CRITICAL();
	hydradancer_status.ep_in_nak &= ~(1 << endp_num);
	BSP_EXIT_CRITICAL();
}

#endif
