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
#include "usb_device.h"
#include "usb_handlers.h"
#include "wch-ch56x-lib/HSPIDeviceScheduled/hspi_scheduled.h"
#include "wch-ch56x-lib/interrupt_queue/interrupt_queue.h"
#include "wch-ch56x-lib/logging/log_to_buffer.h"
#include "wch-ch56x-lib/memory/ramx_alloc.h"
#include "wch-ch56x-lib/USBDevice/usb20.h"
#include "wch-ch56x-lib/USBDevice/usb2_utils.h"
#include "wch-ch56x-lib/USBDevice/usb30.h"
#include "wch-ch56x-lib/USBDevice/usb_descriptors.h"
#include "wch-ch56x-lib/USBDevice/usb_endpoints.h"

void reinit_emulated_board(void);
void reinit_emulated_board(void)
{
	// // uncomment to empty log_buffer on emulated device disconnection
	// LOG_DUMP();

	bsp_disable_interrupt();
	// Setup ramx pool and interrupt_queue
	hydra_interrupt_queue_free_all();
	ramx_pool_init(); // before interrupt_queue, in case scheduled operations use the block pool
	hydra_interrupt_queue_init();
	hspi_reinit_buffers();
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
	init_endpoints();
	usb2_setup_endpoints();
	usb2_set_device_address(0);
	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}
	hspi_send(&placeholder, 1, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_EMULATION_BOARD_RESET << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
	bsp_enable_interrupt();
}

void do_bus_reset(void);
void do_bus_reset(void)
{
	bsp_disable_interrupt();
	hydradancer_status.ep_in_status = 0;
	hydradancer_status.ep_out_status = 0;
	// Setup ramx pool and interrupt_queue
	hydra_interrupt_queue_free_all();
	ramx_pool_init(); // before interrupt_queue, in case scheduled operations use the block pool
	hydra_interrupt_queue_init();
	hspi_reinit_buffers();
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
	init_endpoints();
	usb2_setup_endpoints();
	for (int i = 0; i < MAX_ENDPOINTS_SUPPORTED; ++i)
	{
		endp_rx_set_state(&usb_device_0, i, ENDP_STATE_ACK);
	}
	hspi_send(&placeholder, 1, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_EMULATION_BOARD_RESET << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
	bsp_enable_interrupt();
}

void hspi_rx_callback(uint8_t* buffer, uint16_t size, uint16_t custom_register);
void hspi_rx_callback(uint8_t* buffer, uint16_t size, uint16_t custom_register)
{
	uint8_t endpoint = custom_register & CUSTOM_REGISTER_ENDPOINT_MASK;
	bool request_code_bit = custom_register & CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK;
	uint8_t request_code = (custom_register & CUSTOM_REGISTER_REQUEST_CODE_MASK) >> CUSTOM_REGISTER_REQUEST_CODE_OFFSET;
	LOG_IF_LEVEL(LOG_LEVEL_DEBUG, "hspi_rx_callback buffer %p size %d ep %d request_code_bit %d request_code %d register %d \r\n", buffer, size, endpoint, request_code_bit, request_code, custom_register);

	if (!request_code_bit)
	{
		switch (endpoint)
		{
		case 0:
			endp_tx_set_new_buffer(&usb_device_0, 0, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 0);
			break;
		case 1:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 1, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 1);
			break;
		case 2:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 2, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 2);
			break;
		case 3:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 3, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 3);
			break;
		case 4:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 4, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 4);
			break;
		case 5:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 5, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 5);
			break;
		case 6:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 6, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 6);
			break;
		case 7:
			ramx_take_ownership(buffer);
			endp_tx_set_new_buffer(&usb_device_0, 7, buffer, size);
			hydradancer_status.ep_in_status &= ~(0x01 << 7);
			break;
		}
	}
	else
	{
		if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_ENABLE_USB_CONNECTION_REQUEST_CODE)
		{
			LOG_IF_LEVEL(LOG_LEVEL_DEBUG, "CONNECTION_REQUEST \r\n");
			usb2_device_init();
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_DISABLE_USB)
		{
			LOG_IF_LEVEL(LOG_LEVEL_DEBUG, "DISCONNECT REQUEST \r\n");
			usb2_device_deinit();
			reinit_emulated_board();
			return;
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_SET_SPEED)
		{
			switch (buffer[0])
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
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_SET_EP_RESPONSE)
		{
			if (buffer[0] & 0x80)
			{
				endp_tx_set_state(&usb_device_0, buffer[0] & 0x7f, buffer[1]);
			}
			else
			{
				endp_rx_set_state(&usb_device_0, buffer[0] & 0x7f, buffer[1]);
			}
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_SET_ADDRESS_REQUEST_CODE)
		{
			LOG_IF_LEVEL(LOG_LEVEL_DEBUG, "Set address %d \r\n", buffer[0]);
			usb2_set_device_address(buffer[0]);
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_SET_ENDPOINT_MAPPING)
		{
			if ((buffer[0] <= MAX_ENDPOINTS_SUPPORTED) && (buffer[1] <= MAX_ENDPOINTS_SUPPORTED))
			{
				LOG_IF_LEVEL(LOG_LEVEL_DEBUG, "SET_ENDPOINT_MAPPING target_ep %d pc_ep %d \r\n", buffer[0], buffer[1]);
				hydradancer_status.ep_in_status |= (0x01 << buffer[0]);
				hydradancer_status.ep_out_status &= ~(0x01 << buffer[0]);
				endpoint_mapping[buffer[0]] = buffer[1];
			}
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_UPDATE_EP_OUT_STATUS)
		{
			hydradancer_status.ep_out_status = (hydradancer_status.ep_out_status & ~(0x1 << buffer[0])) | ((buffer[1] & 0x1) << buffer[0]);
			if (!(hydradancer_status.ep_out_status & (0x01 << buffer[0])))
			{
				endp_rx_set_state(&usb_device_0, buffer[0], ENDP_STATE_ACK);
			}
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_UPDATE_EP_IN_STATUS)
		{
			hydradancer_status.ep_in_status = (hydradancer_status.ep_in_status & ~(0x1 << buffer[0])) | ((buffer[1] & 0x1) << buffer[0]);
		}
		else if (endpoint == 0 && request_code_bit && request_code == HSPI_SERDES_UPDATE_EP_STATUS)
		{
			hydradancer_status = *(hydradancer_status_t*)buffer;
		}
	}
}

void hspi_err_crc_num_mismatch_callback(void);
void hspi_err_crc_num_mismatch_callback(void)
{
	hspi_init(HSPI_TYPE_DEVICE, HSPI_DATASIZE_32, ENDP_1_15_MAX_PACKET_SIZE);
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

	/* Configure watchdog to detect deadlocks*/
	PFIC_EnableIRQ(WDOG_IRQn);
	WWDG_SetCounter(0);
	WWDG_ITCfg(ENABLE);

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

	LOG_INIT(FREQ_SYS);

	/******************************************/
	/* Start Synchronization between 2 Boards */
	/* J3 MOSI(PA14) & J3 SCS(PA12) signals   */
	/******************************************/
	if (bsp_switch() == 0)
	{
		is_board1 = false;
		err = bsp_sync2boards(PA14, PA12, BSP_BOARD2);
	}
	else
	{
		is_board1 = true;
		err = bsp_sync2boards(PA14, PA12, BSP_BOARD1);
	}
	if (err > 0)
		LOG("SYNC %08d\n", err);
	else
		LOG("SYNC Err Timeout\n");
	log_time_init(); // Reinit log time after synchro
	/* Test Synchronization to be checked with Oscilloscope/LA */
	bsp_uled_on();
	bsp_uled_off();

	/****************************************/
	/* End Synchronization between 2 Boards */
	/****************************************/

	/*
	Configure function ptrs
	*/

	usb_device_0.endpoints.endp0_passthrough_setup_callback = endp0_passthrough_setup_callback;
	usb_device_0.endpoints.tx_complete[0] = endp0_tx_complete;
	usb_device_0.endpoints.tx_complete[1] = endp1_tx_complete;
	usb_device_0.endpoints.tx_complete[2] = endp2_tx_complete;
	usb_device_0.endpoints.tx_complete[3] = endp3_tx_complete;
	usb_device_0.endpoints.tx_complete[4] = endp4_tx_complete;
	usb_device_0.endpoints.tx_complete[5] = endp5_tx_complete;
	usb_device_0.endpoints.tx_complete[6] = endp6_tx_complete;
	usb_device_0.endpoints.tx_complete[7] = endp7_tx_complete;
	usb_device_0.endpoints.rx_callback[0] = endp0_rx_callback;
	usb_device_0.endpoints.rx_callback[1] = endp1_rx_callback;
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
	init_usb2_descriptors();
	init_string_descriptors();

	usb_device_0.speed = USB2_HIGHSPEED;
	usb_device_set_usb3_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb2_device_descriptor(&usb_device_0, NULL);
	usb_device_set_usb3_config_descriptors(&usb_device_0, NULL);
	usb_device_set_usb2_config_descriptors(&usb_device_0, NULL);
	usb_device_set_bos_descriptor(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	usb_device_set_string_descriptors(&usb_device_0, NULL);
	// Set the USB device parameters
	usb_device_set_usb2_device_descriptor(&usb_device_0, &usb2_descriptors.usb_device_descr);
	usb_device_set_usb2_config_descriptors(&usb_device_0, usb2_device_configs);
	usb_device_set_string_descriptors(&usb_device_0, device_string_descriptors);
	usb2_ep0_passthrough_enabled(false);
	usb_device_set_endpoint_mask(&usb_device_0, ENDPOINT_1_RX | ENDPOINT_1_TX | ENDPOINT_2_RX | ENDPOINT_2_TX | ENDPOINT_3_RX | ENDPOINT_3_TX | ENDPOINT_4_RX | ENDPOINT_4_TX | ENDPOINT_5_RX | ENDPOINT_5_TX | ENDPOINT_6_RX | ENDPOINT_6_TX | ENDPOINT_7_RX | ENDPOINT_7_TX);

	init_endpoints();

	hydradancer_status.ep_in_status = 0;
	hydradancer_status.ep_out_status = 0;

	hspi_init(HSPI_TYPE_DEVICE, HSPI_DATASIZE_32, 512);
	LOG("emulated board");

	// Infinite loop USB2/USB3 managed with Interrupt
	while (1)
	{
		WWDG_SetCounter(0); // rearm the watchdog
		hydra_interrupt_queue_run();

		if (bsp_ubtn())
		{
			blink_ms = BLINK_FAST;
			bsp_uled_on();
			bsp_wait_ms_delay(blink_ms);
			bsp_uled_off();
			bsp_wait_ms_delay(blink_ms);

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
