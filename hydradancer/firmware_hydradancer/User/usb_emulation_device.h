#ifndef USB_EMULATION_DEVICE_H
#define USB_EMULATION_DEVICE_H

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

#include "wch-ch56x-lib/interrupt_queue/interrupt_queue.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_device.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

#define USB2_ENDP_1_15_MAX_PACKET_SIZE 1024

void usb_emulation_reinit(void);

void usb_emulation_init_endpoints(void);

#endif
