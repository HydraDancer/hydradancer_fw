#ifndef LOG_TO_CONTROL_H
#define LOG_TO_CONTROL_H
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

#define CUSTOM_REGISTER_ENDPOINT_MASK 0x000f
#define CUSTOM_REGISTER_REQUEST_CODE_BIT_MASK 0x0010
#define CUSTOM_REGISTER_REQUEST_CODE_MASK 0x01e0
#define CUSTOM_REGISTER_REQUEST_CODE_OFFSET 0x5

#define HSPI_SERDES_LOG_EMULATED_BOARD 0x05

void log_to_control(const char* fmt, ...);

#endif
