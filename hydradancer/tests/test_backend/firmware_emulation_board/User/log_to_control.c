#include "log_to_control.h"
#include "wch-ch56x-lib/logging/logging.h"
#include "wch-ch56x-lib/serdes/serdes.h"

#include "CH56x_common.h"
#include <stdarg.h>

#define LOG_TO_CONTROL_BUF_SIZE (512)

__attribute__((aligned(16))) uint8_t log_to_control_buffer[LOG_TO_CONTROL_BUF_SIZE] __attribute__((section(".DMADATA")));

void log_to_control(const char* fmt, ...)
{
	va_list va_args;
	int print_size;

	// bsp_disable_interrupt(); // Enter Critical Section
	va_start(va_args, fmt);
	print_size = vsnprintf((char*)log_to_control_buffer, LOG_TO_CONTROL_BUF_SIZE, fmt, va_args);
	va_end(va_args);
	if (print_size > 0)
	{
		LOG("%d \r\n %x", print_size, log_to_control_buffer);
		LOG("%s", log_to_control_buffer);
		serdes_send(log_to_control_buffer, print_size, (0 & CUSTOM_REGISTER_ENDPOINT_MASK) | CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK | (HSPI_SERDES_LOG_EMULATED_BOARD << CUSTOM_REGISTER_REQUEST_CODE_OFFSET));
	}
	// bsp_enable_interrupt(); // Exit Critical Section
}
