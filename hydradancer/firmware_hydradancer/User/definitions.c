#include "definitions.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"

uint8_t endpoint_mapping_reverse[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping_reverse[PC_Endpoint] = Target_Endpoint
uint8_t endpoint_mapping[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping[Target_Endpoint] = PC_Endpoint

__attribute__((aligned(16))) volatile hydradancer_status_t hydradancer_status __attribute__((section(".DMADATA"))) = { .ep_in_nak = 0, .ep_in_status = 0, .ep_out_status = 0, .other_events = 0 };
__attribute__((aligned(16))) volatile uint8_t boards_ready __attribute__((section(".DMADATA"))) = 0;
__attribute__((aligned(16))) volatile bool event_transfer_finished = true;
__attribute__((aligned(16))) volatile bool start_polling = false;
__attribute__((aligned(16))) volatile hydradancer_event_t _events_buffer[EVENT_QUEUE_SIZE] __attribute__((section(".DMADATA")));

HYDRA_POOL_DEF(ep_queue, ep_queue_member_t, 100);

void _ep_queue_cleanup(uint8_t* data)
{
	hydra_pool_free(&ep_queue, data);
}

HYDRA_FIFO_DEF(event_queue, hydradancer_event_t, EVENT_QUEUE_SIZE);

void hydradancer_send_event(void)
{
	if (event_transfer_finished)
	{
		uint16_t events_count = fifo_count(&event_queue);
		if (events_count > 0)
		{
			event_transfer_finished = false;
			uint16_t count_read = fifo_read_n(&event_queue, (void*)_events_buffer, events_count);
			endp_tx_set_new_buffer(&usb_device_1, 1, (uint8_t*)_events_buffer, count_read * sizeof(hydradancer_event_t));
		}
	}
}
