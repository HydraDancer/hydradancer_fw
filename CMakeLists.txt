cmake_minimum_required(VERSION 3.17)
project(hydradancer LANGUAGES C ASM)

## Adding wch-ch56-lib

add_subdirectory(submodules/wch-ch56x-lib)

## Add hydradancer

add_subdirectory(hydradancer)

if (DEFINED BUILD_LEGACY)
add_subdirectory(legacy)
endif()
