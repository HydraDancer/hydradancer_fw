#include "definitions.h"

uint8_t endpoint_mapping_reverse[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping_reverse[PC_Endpoint] = Target_Endpoint
uint8_t endpoint_mapping[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping[Target_Endpoint] = PC_Endpoint

__attribute__((aligned(16))) volatile hydradancer_status_t hydradancer_status __attribute__((section(".DMADATA"))) = { .ep_in_nak = 0, .ep_in_status = 0, .ep_out_status = 0, .other_events = 0 };
__attribute__((aligned(16))) volatile uint8_t boards_ready __attribute__((section(".DMADATA"))) = 0;

void send_hydradancer_status(void)
{
	endp_tx_set_new_buffer(&usb_device_1, 1, (uint8_t*)&hydradancer_status, sizeof(hydradancer_status));
}

HYDRA_POOL_DEF(ep0_queue, ep0_queue_member_t, 20);

void _ep0_queue_cleanup(uint8_t* data)
{
	hydra_pool_free(&ep0_queue, data);
}
