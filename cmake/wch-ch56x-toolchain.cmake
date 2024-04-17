# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)

# Check and choose riscv compiler either closed source from MounRiver Studio riscv-none-embed" or open source one GCC riscv-none-elf
# For open source GCC riscv-none-elf see https://github.com/hydrausb3/riscv-none-elf-gcc-xpack/releases/

find_program(RISCV_GCC_TOOLCHAIN NAMES riscv-none-embed-gcc riscv-none-elf-gcc REQUIRED)

cmake_path(GET RISCV_GCC_TOOLCHAIN FILENAME RISCV_GCC_TOOLCHAIN_PREFIX)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   ${RISCV_GCC_TOOLCHAIN})

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  "")

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
