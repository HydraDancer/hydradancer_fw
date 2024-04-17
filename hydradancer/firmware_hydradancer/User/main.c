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
#include "usb_control_descriptors.h"
#include "usb_control_device.h"
#include "usb_control_handlers.h"
#include "usb_emulation_device.h"
#include "usb_emulation_handlers.h"
#include "wch-ch56x-lib/interrupt_queue/interrupt_queue.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

int blink_ms = BLINK_USB2;

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main()
{
	/* Configure GPIO In/Out default/safe state for the board */
	bsp_gpio_init();
	/* Init BSP (MCU Frequency & SysTick) */
	bsp_init(FREQ_SYS);

	LOG_INIT(FREQ_SYS);
	hydra_interrupt_queue_init();

	// /* Need to check if setting priorities has any effects */
	// PFIC_SetPriority(INT_ID_WDOG, 0x90);
	// // does not seem to make any difference
	// PFIC_INTNestCfg(ENABLE);
	// PFIC_SetPriority(INT_ID_SERDES, 0x20);
	// PFIC_SetPriority(INT_ID_USBSS, 0x60);
	// PFIC_SetPriority(INT_ID_LINK, 0x60);
	// PFIC_SetPriority(INT_ID_USBHS, 0x60);
	// PFIC_SetPriority(INT_ID_HSPI, 0x20);
	// PFIC_SetPriority(INT_ID_UART1, 0x60);
	// PFIC_SetPriority(INT_ID_GPIO, 0x60);

	// Setup ramx pool and interrupt_queue
	ramx_pool_init();

	//=================== Initialize Control device =====================//

	usb_device_1.speed = USB30_SUPERSPEED;
	usb_device_1.endpoints.endp0_user_handled_control_request = usb_control_endp0_user_handled_control_request;
	usb_device_1.endpoints.tx_complete[1] = usb_control_endp1_tx_complete;
	usb_device_1.endpoints.tx_complete[2] = usb_control_endp2_tx_complete;
	usb_device_1.endpoints.tx_complete[3] = usb_control_endp3_tx_complete;
	usb_device_1.endpoints.tx_complete[4] = usb_control_endp4_tx_complete;
	usb_device_1.endpoints.tx_complete[5] = usb_control_endp5_tx_complete;
	usb_device_1.endpoints.tx_complete[6] = usb_control_endp6_tx_complete;
	usb_device_1.endpoints.tx_complete[7] = usb_control_endp7_tx_complete;
	usb_device_1.endpoints.rx_callback[2] = usb_control_endp2_rx_callback;
	usb_device_1.endpoints.rx_callback[3] = usb_control_endp3_rx_callback;
	usb_device_1.endpoints.rx_callback[4] = usb_control_endp4_rx_callback;
	usb_device_1.endpoints.rx_callback[5] = usb_control_endp5_rx_callback;
	usb_device_1.endpoints.rx_callback[6] = usb_control_endp6_rx_callback;
	usb_device_1.endpoints.rx_callback[7] = usb_control_endp7_rx_callback;

	//Finish initializing the descriptor parameters
	usb_control_init_descriptors();
	usb_control_init_string_descriptors();
	usb_control_init_endpoints();

	// Set the USB device parameters
	usb_device_set_usb3_device_descriptor(&usb_device_1, &usb_control_descriptors.usb_device_descr);
	usb_device_set_usb3_config_descriptors(&usb_device_1, usb_control_device_configs);
	usb_device_set_bos_descriptor(&usb_device_1, &usb_control_descriptors.capabilities.usb_bos_descr);
	usb_device_set_string_descriptors(&usb_device_1, device_string_descriptors);
	usb_device_set_endpoint_mask(&usb_device_1, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);

	//=================== Initialize Emulation device =====================//

	/*
	Configure function ptrs
	*/

	usb_device_0.endpoints.endp0_passthrough_setup_callback = usb_emulation_endp0_passthrough_setup_callback;
	usb_device_0.endpoints.tx_complete[0] = usb_emulation_endp0_tx_complete;
	usb_device_0.endpoints.tx_complete[1] = usb_emulation_endp1_tx_complete;
	usb_device_0.endpoints.tx_complete[2] = usb_emulation_endp2_tx_complete;
	usb_device_0.endpoints.tx_complete[3] = usb_emulation_endp3_tx_complete;
	usb_device_0.endpoints.tx_complete[4] = usb_emulation_endp4_tx_complete;
	usb_device_0.endpoints.tx_complete[5] = usb_emulation_endp5_tx_complete;
	usb_device_0.endpoints.tx_complete[6] = usb_emulation_endp6_tx_complete;
	usb_device_0.endpoints.tx_complete[7] = usb_emulation_endp7_tx_complete;
	usb_device_0.endpoints.rx_callback[0] = usb_emulation_endp0_rx_callback;
	usb_device_0.endpoints.rx_callback[1] = usb_emulation_endp1_rx_callback;
	usb_device_0.endpoints.rx_callback[2] = usb_emulation_endp2_rx_callback;
	usb_device_0.endpoints.rx_callback[3] = usb_emulation_endp3_rx_callback;
	usb_device_0.endpoints.rx_callback[4] = usb_emulation_endp4_rx_callback;
	usb_device_0.endpoints.rx_callback[5] = usb_emulation_endp5_rx_callback;
	usb_device_0.endpoints.rx_callback[6] = usb_emulation_endp6_rx_callback;
	usb_device_0.endpoints.rx_callback[7] = usb_emulation_endp7_rx_callback;

	usb2_user_handled.usb2_device_handle_bus_reset = usb_emulation_usb2_device_handle_bus_reset;

	usb_device_0.speed = USB2_HIGHSPEED;
	usb_device_set_usb3_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb2_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb3_config_descriptors(&usb_device_0, NULL);
	usb_device_set_usb2_config_descriptors(&usb_device_0, NULL);
	usb_device_set_bos_descriptor(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	// Set the USB device parameters
	usb2_ep0_passthrough_enabled(true);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);
	usb_emulation_init_endpoints();

	//=================== Finished initializing Emulation device =====================//

	boards_ready = 1;

	usb3_backend_current_device = &usb_device_1; // the USB3 backend will handle the control device
	usb2_backend_current_device = &usb_device_0; // the USB2 backend will create the Facedancer device (emulation device)

	usb30_device_init(false);

	/* Configure watchdog to detect deadlocks*/
	PFIC_EnableIRQ(WDOG_IRQn);
	WWDG_SetCounter(0);
	WWDG_ITCfg(ENABLE);

	// Infinite loop USB2/USB3 managed with Interrupt
	while (1)
	{
		WWDG_SetCounter(0); // rearm the watchdog
		hydra_interrupt_queue_run();
		if (bsp_ubtn())
		{
			LOG_DUMP();
		}
	}
}

__attribute__((interrupt("WCH-Interrupt-fast"))) void WDOG_IRQHandler(void);
__attribute__((interrupt("WCH-Interrupt-fast"))) void WDOG_IRQHandler(void)
{
	LOG_DUMP();

	LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "WDOG_IRQHandler\r\n"
									 " SP=0x%08X\r\n"
									 " MIE=0x%08X\r\n"
									 " MSTATUS=0x%08X\r\n"
									 " MCAUSE=0x%08X\r\n"
									 " MVENDORID=0x%08X\r\n"
									 " MARCHID=0x%08X\r\n"
									 " MISA=0x%08X\r\n"
									 " MIMPID=0x%08X\r\n"
									 " MHARTID=0x%08X\r\n"
									 " MEPC=0x%08X\r\n"
									 " MSCRATCH=0x%08X\r\n"
									 " MTVEC=0x%08X\r\n",
				 __get_SP(),
				 __get_MIE(),
				 __get_MSTATUS(),
				 __get_MCAUSE(),
				 __get_MVENDORID(),
				 __get_MARCHID(),
				 __get_MISA(),
				 __get_MIMPID(),
				 __get_MHARTID(),
				 __get_MEPC(),
				 __get_MSCRATCH(),
				 __get_MTVEC());

	LOG_DUMP();

	bsp_wait_ms_delay(100000000);
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   Example of basic HardFault Handler called if an exception occurs
 *
 * @return  none
 */
__attribute__((interrupt("WCH-Interrupt-fast"))) void HardFault_Handler(void);
__attribute__((interrupt("WCH-Interrupt-fast"))) void HardFault_Handler(void)
{
	LOG_DUMP();

	// asm("ebreak"); to trigger a breakpoint and test hardfault_handler
	LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "HardFault_Handler\r\n"
									 " SP=0x%08X\r\n"
									 " MIE=0x%08X\r\n"
									 " MSTATUS=0x%08X\r\n"
									 " MCAUSE=0x%08X\r\n"
									 " MVENDORID=0x%08X\r\n"
									 " MARCHID=0x%08X\r\n"
									 " MISA=0x%08X\r\n"
									 " MIMPID=0x%08X\r\n"
									 " MHARTID=0x%08X\r\n"
									 " MEPC=0x%08X\r\n"
									 " MSCRATCH=0x%08X\r\n"
									 " MTVEC=0x%08X\r\n",
				 __get_SP(),
				 __get_MIE(),
				 __get_MSTATUS(),
				 __get_MCAUSE(),
				 __get_MVENDORID(),
				 __get_MARCHID(),
				 __get_MISA(),
				 __get_MIMPID(),
				 __get_MHARTID(),
				 __get_MEPC(),
				 __get_MSCRATCH(),
				 __get_MTVEC());

	LOG_DUMP();

	bsp_wait_ms_delay(100000000);
}
