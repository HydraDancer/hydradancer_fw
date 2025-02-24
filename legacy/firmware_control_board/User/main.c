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
#include "usb2_device_descriptors.h"
#include "usb3_device_descriptors.h"
#include "usb_device.h"
#include "usb_handlers.h"
#include "wch-ch56x-lib/hspi_scheduled/hspi_scheduled.h"
#include "wch-ch56x-lib/interrupt_queue/interrupt_queue.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/usb/usb30.h"
#include "wch-ch56x-lib/usb/usb_descriptors.h"
#include "wch-ch56x-lib/usb/usb_endpoints.h"

int blink_ms = BLINK_USB2;

bool reinit_board(uint8_t* data);
bool reinit_board(uint8_t* data)
{
	// // uncomment to empty log_buffer on emulated device disconnection
	// LOG_DUMP();

	bsp_disable_interrupt();
	hydra_interrupt_queue_free_all();
	ramx_pool_init();
	hydra_interrupt_queue_init();
	hspi_reinit_buffers();
	init_endpoints();
	usb30_init_endpoints(); // update USB3 backend as well, without rebooting USB3 peripheral
	boards_ready = 1;
	hydradancer_status.ep_in_status = 0;
	hydradancer_status.ep_out_status = 0;
	hydradancer_status.ep_in_nak = 0;
	hydradancer_status.other_events = 0;
	bsp_enable_interrupt();
	return true;
}

void hspi_rx_callback(uint8_t* buffer, uint16_t size, uint16_t custom_register);
void hspi_rx_callback(uint8_t* buffer, uint16_t size, uint16_t custom_register)
{
	uint8_t endpoint = custom_register & CUSTOM_REGISTER_ENDPOINT_MASK;
	bool request_code_bit = custom_register & CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK;
	uint8_t request_code = (custom_register & CUSTOM_REGISTER_REQUEST_CODE_MASK) >> CUSTOM_REGISTER_REQUEST_CODE_OFFSET;

	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "hspi_rx_callback buffer %p size %d ep %d %d %d %d %d %d \r\n", buffer, size, custom_register & CUSTOM_REGISTER_ENDPOINT_MASK, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
	if (!request_code_bit && boards_ready)
	{
		switch (endpoint)
		{
		case 2:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[2]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 2, buffer, size);
			send_hydradancer_status();
			break;
		case 3:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[3]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 3, buffer, size);
			send_hydradancer_status();
			break;
		case 4:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[4]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 4, buffer, size);
			send_hydradancer_status();
			break;
		case 5:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[5]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 5, buffer, size);
			send_hydradancer_status();
			break;
		case 6:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[6]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 6, buffer, size);
			send_hydradancer_status();
			break;
		case 7:
			hydradancer_status.ep_out_status |= (0x01 << endpoint_mapping_reverse[7]);
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 7, buffer, size);
			send_hydradancer_status();
			break;
		default:
			return;
		}
	}
	else if (request_code_bit)
	{
		if (request_code == HSPI_SERDES_EMULATION_BOARD_RESET)
		{
			LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "HSPI_SERDES_EMULATION_BOARD_RESET\r\n");
			hydra_interrupt_queue_set_next_task(reinit_board, NULL, NULL);
			return;
		}
		else if (request_code == HSPI_SERDES_EMULATION_BOARD_BUS_RESET)
		{
			LOG("HSPI_SERDES_EMULATION_BOARD_RESET\r\n");
			hydradancer_status.other_events |= HYDRADANCER_STATUS_BUS_RESET;
			send_hydradancer_status();
			return;
		}
		else if (request_code == HSPI_SERDES_LOG_EMULATED_BOARD)
		{
			LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "received logs from emulated board %d size\r\n", size);
			buffer[size] = 0;
			LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "--> emulated board : %s\r\n", (char*)buffer);
		}
		else if (request_code == HSPI_SERDES_UPDATE_EP_OUT_STATUS && boards_ready)
		{
			hydradancer_status.ep_out_status = (hydradancer_status.ep_in_status & ~(0x1 << buffer[0])) | ((buffer[1] & 0x1) << buffer[0]);
		}
		else if (request_code == HSPI_SERDES_UPDATE_EP_IN_STATUS && boards_ready)
		{
			hydradancer_status.ep_in_status = (hydradancer_status.ep_in_status & ~(0x1 << buffer[0])) | ((buffer[1] & 0x1) << buffer[0]);

			if (buffer[1] == 1)
			{
				send_hydradancer_status();
			}
		}
		else if (request_code == HSPI_SERDES_UPDATE_EP_STATUS)
		{
			hydradancer_status = *(hydradancer_status_t*)(buffer);
		}
	}
	LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "hspi_rx_callback end \r\n");
}

void hspi_err_crc_num_mismatch_callback(void);
void hspi_err_crc_num_mismatch_callback(void)
{
	hspi_init(HSPI_TYPE_HOST, HSPI_DATASIZE_32, ENDP_1_15_MAX_PACKET_SIZE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main()
{
	uint32_t err;

	/* Configure GPIO In/Out default/safe state for the board */
	bsp_gpio_init();
	/* Init BSP (MCU Frequency & SysTick) */
	bsp_init(FREQ_SYS);

	LOG_INIT(FREQ_SYS);

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

	/******************************************/
	/* Start Synchronization between 2 Boards */
	/* J3 MOSI(PA14) & J3 SCS(PA12) signals   */
	/******************************************/
	err = bsp_sync2boards(PA14, PA12, BSP_BOARD2);

	if (err > 0)
		LOG_IF(LOG_LEVEL_DEBUG, LOG_ID_USER, "SYNC %08d\n", err);
	else
		LOG_IF_LEVEL(LOG_LEVEL_CRITICAL, "SYNC Err Timeout\n");
	log_time_init(); // Reinit log time after synchro
	/* Test Synchronization to be checked with Oscilloscope/LA */
	bsp_uled_on();
	bsp_uled_off();
	/****************************************/
	/* End Synchronization between 2 Boards */
	/****************************************/

	LOG_IF_LEVEL(LOG_LEVEL_INFO, "control board\r\n");

	usb_device_0.speed = USB30_SUPERSPEED;
	usb_device_0.endpoints.endp0_user_handled_control_request = endp0_user_handled_control_request;
	usb_device_0.endpoints.tx_complete[1] = endp1_tx_complete;
	usb_device_0.endpoints.tx_complete[2] = endp2_tx_complete;
	usb_device_0.endpoints.tx_complete[3] = endp3_tx_complete;
	usb_device_0.endpoints.tx_complete[4] = endp4_tx_complete;
	usb_device_0.endpoints.tx_complete[5] = endp5_tx_complete;
	usb_device_0.endpoints.tx_complete[6] = endp6_tx_complete;
	usb_device_0.endpoints.tx_complete[7] = endp7_tx_complete;
	usb_device_0.endpoints.rx_callback[2] = endp2_rx_callback;
	usb_device_0.endpoints.rx_callback[3] = endp3_rx_callback;
	usb_device_0.endpoints.rx_callback[4] = endp4_rx_callback;
	usb_device_0.endpoints.rx_callback[5] = endp5_rx_callback;
	usb_device_0.endpoints.rx_callback[6] = endp6_rx_callback;
	usb_device_0.endpoints.rx_callback[7] = endp7_rx_callback;

	hspi_scheduled_user_handled.hspi_rx_callback = hspi_rx_callback;
	hspi_scheduled_user_handled.hspi_err_crc_num_mismatch_callback = hspi_err_crc_num_mismatch_callback;

	// Setup ramx pool and interrupt_queue
	ramx_pool_init(); // before interrupt_queue, in case scheduled operations use the block pool
	hydra_interrupt_queue_init();

	//Finish initializing the descriptor parameters
	init_usb3_descriptors();
	init_usb2_descriptors();
	init_string_descriptors();

	// Set the USB device parameters
	usb_device_set_usb3_device_descriptor(&usb_device_0, &usb3_descriptors.usb_device_descr);
	usb_device_set_usb2_device_descriptor(&usb_device_0, &usb2_descriptors.usb_device_descr);
	usb_device_set_usb3_config_descriptors(&usb_device_0, usb3_device_configs);
	usb_device_set_usb2_config_descriptors(&usb_device_0, usb2_device_configs);
	usb_device_set_bos_descriptor(&usb_device_0, &usb3_descriptors.capabilities.usb_bos_descr);
	usb_device_set_string_descriptors(&usb_device_0, device_string_descriptors);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);
	init_endpoints();

	hspi_init(HSPI_TYPE_HOST, HSPI_DATASIZE_32, 512);

	boards_ready = 1;

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
