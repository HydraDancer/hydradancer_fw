
project(firmware_hydradancer LANGUAGES C)
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/User/main.c
    ${CMAKE_CURRENT_LIST_DIR}/User/definitions.c
    ${CMAKE_CURRENT_LIST_DIR}/User/usb_control_device.c
    ${CMAKE_CURRENT_LIST_DIR}/User/usb_emulation_device.c
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/User)

##### Define program options

#### wch-ch56x-lib options

target_compile_definitions(${PROJECT_NAME} PRIVATE POOL_BLOCK_SIZE=512 POOL_BLOCK_NUM=100 INTERRUPT_QUEUE_SIZE=40)

#### logging options

# set(LOG_OUTPUT "printf")
# set(LOG_LEVEL 1)
# set(LOG_FILTER_IDS "1")

if (DEFINED LOG_OUTPUT)
    if (${LOG_OUTPUT} STREQUAL "buffer")
        target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_TYPE_BUFFER=1)
    endif()
    if (${LOG_OUTPUT} STREQUAL "serdes")
        target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_TYPE_SERDES=1)
    endif()
    if (${LOG_OUTPUT} STREQUAL "printf")
        target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_TYPE_PRINTF=1)
    endif()
endif()

if (DEFINED LOG_LEVEL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_LEVEL=${LOG_LEVEL})
endif()

if (DEFINED LOG_FILTER_IDS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LOG_FILTER_IDS=${LOG_FILTER_IDS})
endif()

if (DEFINED STATIC_ANALYSIS)
    target_compile_options(${PROJECT_NAME} PRIVATE -fanalyzer)
endif()

##### Compilation and linkage options

target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror -Wno-comment -pedantic -Wall -Wno-error=unused-parameter -Wno-error=unused-function
    -Wbad-function-cast -Wredundant-decls -Wmissing-prototypes -Wchar-subscripts -Wshadow -Wundef -Wwrite-strings -Wunused -Wuninitialized -Wpointer-arith -Winline -Wformat -Wformat-security -Winit-self -Wmissing-include-dirs -Wnested-externs -Wmissing-declarations -Wempty-body -Wignored-qualifiers -Wmissing-field-initializers -Wtype-limits -Wcast-align -Wswitch-enum
    -Wextra -Wclobbered -Wcast-function-type -Wimplicit-fallthrough=3 -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wshift-negative-value -Wunused-but-set-parameter
)

if (DEFINED EXTRACFLAGS)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wunused-parameter -Wno-error=unused-parameter
	-Wsign-compare -Wno-error=sign-compare
	-Wconversion -Wno-error=conversion -Wno-error=sign-conversion -Wno-error=float-conversion
)
endif()

if (${RISCV_GCC_TOOLCHAIN_PREFIX} STREQUAL "riscv-none-embed-gcc")
    message("Using riscv-none-embed-gcc")
    target_compile_options(${PROJECT_NAME} PRIVATE -march=rv32imac)
elseif(${RISCV_GCC_TOOLCHAIN_PREFIX} STREQUAL "riscv-none-elf-gcc")
    message("Using riscv-none-elf-gcc")
    target_compile_options(${PROJECT_NAME} PRIVATE -march=rv32imac_zicsr)
else()
    message("Toolchain not found")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -std=gnu99 -MMD -MP -mabi=ilp32 -msmall-data-limit=8 -fmessage-length=0 -finline-limit=2000 -fsigned-char -ffunction-sections -fdata-sections)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-Og> $<$<CONFIG:Release>:-Oz>)
target_link_options(${PROJECT_NAME} PRIVATE -T "${CMAKE_CURRENT_LIST_DIR}/.ld" -nostartfiles  LINKER:--gc-sections LINKER:--print-memory-usage -Wl,-Map,${PROJECT_NAME}.map --specs=nano.specs --specs=nosys.specs)
target_link_libraries(${PROJECT_NAME} wch-ch56x-lib-scheduled)

##### Generate additional targets

add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_target(${PROJECT_NAME}.lst ALL DEPENDS ${PROJECT_NAME}.elf)

add_custom_command(TARGET ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf
    ${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf
    ${PROJECT_NAME}.hex)

add_custom_command(TARGET ${PROJECT_NAME}.lst
    COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst)

##### Export generated files

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin DESTINATION ${CMAKE_SOURCE_DIR}/out/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex DESTINATION ${CMAKE_SOURCE_DIR}/out/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf DESTINATION ${CMAKE_SOURCE_DIR}/out/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.lst DESTINATION ${CMAKE_SOURCE_DIR}/out/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map DESTINATION ${CMAKE_SOURCE_DIR}/out/${PROJECT_NAME})
