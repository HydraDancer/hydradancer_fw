
add_subdirectory(firmware_control_board)
add_subdirectory(firmware_emulation_board)
add_subdirectory(firmware_hydradancer)

if (DEFINED BUILD_TESTS)
add_subdirectory(tests)
endif()
