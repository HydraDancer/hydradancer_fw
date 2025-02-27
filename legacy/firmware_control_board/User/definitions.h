#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define ENDP_1_15_MAX_PACKET_SIZE 1024

/* Global define */
// DEF_ENDP_OUT_BURST_LEVEL / DEF_ENDP_IN_BURST_LEVEL maximum burst size 16 defined by the USB3 specification
// Warning USB3 endpoint bulk with 8 or 16 burst can be problematic on some PC
#define DEF_ENDP_OUT_BURST_LEVEL 4
#define DEF_ENDP_IN_BURST_LEVEL (DEF_ENDP_OUT_BURST_LEVEL)
#define DEF_ENDP_MAX_SIZE (DEF_ENDP1_OUT_BURST_LEVEL * ENDP_1_15_MAX_PACKET_SIZE)

#include "wch-ch56x-lib/usb/usb_device.h"
#include "wch-ch56x-lib/usb/usb_endpoints.h"
typedef struct __attribute__((packed))
{
	uint8_t ep_in_status;
	uint8_t ep_out_status;
	uint8_t ep_in_nak;
	uint8_t other_events;
} hydradancer_status_t;

__attribute__((aligned(16))) extern volatile uint8_t boards_ready __attribute__((section(".DMADATA")));

#define CUSTOM_REGISTER_ENDPOINT_MASK 0x000f
#define CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK 0x0010
#define CUSTOM_REGISTER_REQUEST_CODE_MASK 0x01e0
#define CUSTOM_REGISTER_REQUEST_CODE_OFFSET 0x5

// Standard control requests codes go from 0-27 and 48-49
#define ENABLE_USB_CONNECTION_REQUEST_CODE 50
#define SET_ADDRESS_REQUEST_CODE 51
#define GET_EP_STATUS 52
#define SET_ENDPOINT_MAPPING 53
#define DISABLE_USB 54
#define SET_SPEED 55
#define SET_EP_RESPONSE 56
#define CHECK_HYDRADANCER_READY 57
#define DO_BUS_RESET 58

// There is very limited space in HSPI custom_number, use smaller numbers to map the vendor usb control requests.
#define HSPI_SERDES_ENABLE_USB_CONNECTION_REQUEST_CODE 0x01
#define HSPI_SERDES_SET_ADDRESS_REQUEST_CODE 0x02
#define HSPI_SERDES_GET_IN_ENDPOINTS_STATUS 0x03
#define HSPI_SERDES_SET_ENDPOINT_MAPPING 0x04
#define HSPI_SERDES_LOG_EMULATED_BOARD 0x05
#define HSPI_SERDES_UPDATE_EP_OUT_STATUS 0x06
#define HSPI_SERDES_UPDATE_EP_IN_STATUS 0x07
#define HSPI_SERDES_UPDATE_EP_STATUS 0x08
#define HSPI_SERDES_DISABLE_USB 0x09
#define HSPI_SERDES_SET_SPEED 0x0a
#define HSPI_SERDES_SET_EP_RESPONSE 0x0b
#define HSPI_SERDES_EMULATION_BOARD_RESET 0x0c
#define HSPI_SERDES_EMULATION_BOARD_BUS_RESET 0x0d

#define HYDRADANCER_STATUS_BUS_RESET 0x1

#undef FREQ_SYS
/* System clock / MCU frequency in Hz (lowest possible speed 15MHz) */
#define FREQ_SYS (120000000)

/* Blink time in ms */
#define BLINK_FAST (50) // Blink LED each 100ms (50*2)
#define BLINK_USB3 (250) // Blink LED each 500ms (250*2)
#define BLINK_USB2 (1000) // Blink LED each 2000ms (1000*2)

#define MAX_ENDPOINTS_SUPPORTED 8 //including EP0
uint8_t endpoint_mapping_reverse[MAX_ENDPOINTS_SUPPORTED]; //endpoint_mapping_reverse[PC_Endpoint] = Target_Endpoint

__attribute__((aligned(16))) uint8_t buf_0[ENDP_1_15_MAX_PACKET_SIZE * DEF_ENDP_IN_BURST_LEVEL] __attribute__((section(".DMADATA")));
__attribute__((aligned(16))) hydradancer_status_t hydradancer_status __attribute__((section(".DMADATA"))) = { .ep_in_nak = 0, .ep_in_status = 0, .ep_out_status = 0, .other_events = 0 };
__attribute__((aligned(16))) uint8_t ep_status_buffer[2] __attribute__((section(".DMADATA"))) = { 0, 0 };
__attribute__((aligned(16))) volatile uint8_t boards_ready __attribute__((section(".DMADATA"))) = 0;

void send_hydradancer_status(void);
void send_hydradancer_status(void)
{
	endp_tx_set_new_buffer(&usb_device_0, 1, (uint8_t*)&hydradancer_status, sizeof(hydradancer_status));
}

#endif