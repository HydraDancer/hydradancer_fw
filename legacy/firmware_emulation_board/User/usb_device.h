#ifndef USB_DEVICE_USER_H
#define USB_DEVICE_USER_H

#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/usb/usb_descriptors.h"
#include "wch-ch56x-lib/usb/usb_device.h"
#include "wch-ch56x-lib/usb/usb_endpoints.h"

#define ENDP_1_15_MAX_PACKET_SIZE 512

/* Global define */
// DEF_ENDP_OUT_BURST_LEVEL / DEF_ENDP_IN_BURST_LEVEL maximum burst size 16 defined by the USB3 specification
// Warning USB3 endpoint bulk with 8 or 16 burst can be problematic on some PC
#define DEF_ENDP_OUT_BURST_LEVEL 1
#define DEF_ENDP_IN_BURST_LEVEL (DEF_ENDP_OUT_BURST_LEVEL)
#define DEF_ENDP_MAX_SIZE (DEF_ENDP1_OUT_BURST_LEVEL * ENDP_1_15_MAX_PACKET_SIZE)

void init_endpoints(void);
void init_endpoints(void)
{
	usb_device_0.endpoints.rx[0].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[0].max_packet_size = 64;
	usb_device_0.endpoints.rx[0].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[0].max_packet_size_with_burst = 64;

	usb_device_0.endpoints.tx[0].buffer = usb_device_0.endpoints.rx[0].buffer;
	usb_device_0.endpoints.tx[0].max_packet_size = 64;
	usb_device_0.endpoints.tx[0].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[0].max_packet_size_with_burst = 64;

	usb_device_0.endpoints.rx[1].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[1].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[1].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[1].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[1].buffer = NULL;
	usb_device_0.endpoints.tx[1].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[1].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[1].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[2].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[2].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[2].buffer = NULL;
	usb_device_0.endpoints.tx[2].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[2].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[2].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[3].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[3].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[3].buffer = NULL;
	usb_device_0.endpoints.tx[3].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[3].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[3].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[4].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[4].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[4].buffer = NULL;
	usb_device_0.endpoints.tx[4].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[4].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[4].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[5].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[5].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[5].buffer = NULL;
	usb_device_0.endpoints.tx[5].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[5].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[5].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[6].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[6].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[6].buffer = NULL;
	usb_device_0.endpoints.tx[6].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[6].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[6].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.rx[7].buffer = ramx_pool_alloc_bytes(ENDP_1_15_MAX_PACKET_SIZE);
	usb_device_0.endpoints.rx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.rx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.rx[7].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;

	usb_device_0.endpoints.tx[7].buffer = NULL;
	usb_device_0.endpoints.tx[7].max_packet_size = ENDP_1_15_MAX_PACKET_SIZE;
	usb_device_0.endpoints.tx[7].max_burst = DEF_ENDP_OUT_BURST_LEVEL;
	usb_device_0.endpoints.tx[7].max_packet_size_with_burst = ENDP_1_15_MAX_PACKET_SIZE;
}

#endif
