# How to build the firmwares

## Prerequisites

You have two options to get a working compilation environment for Hydradancer : the Docker way or the native way.


### The Docker way (Linux/Windows)

You first need to install Docker : https://docs.docker.com/engine/install/.

Then simply go to the `tools/Docker` folder and launch :

on Linux

```shell
docker build -t hydradancer_compile --build-arg UID=$(id -u) .
```

on Windows
```shell
docker build -t hydradancer_compile --build-arg UID=1000 .
```

from the command line. Then go back to the top folder and execute

```shell
docker run --mount type=bind,source=.,target=/home/user/mounted -it hydradancer_compile
```

The working directory will be the current folder, so you have to be in the folder you want to be mounted inside your Docker instance (for example, the top folder of Hydradancer).

The two scripts `tools/Docker/build.sh` and `tools/Docker/run.sh` respectively will run the same commands on Linux.

### Native way (Linux only)

You will need to install the following tools on your system first : `git` `wget` `tar` `make` `cmake`.

#### GCC RISC-V 12.2 toolchain

Get the latest release from https://github.com/hydrausb3/riscv-none-elf-gcc-xpack/releases , e.g.

```shell
wget https://github.com/hydrausb3/riscv-none-elf-gcc-xpack/releases/download/12.2.0-1/xpack-riscv-none-elf-gcc-12.2.0-2-linux-x64.tar.gz
tar xf xpack-riscv-none-elf-gcc-12.2.0-2-linux-x64.tar.gz
```
To use it, add it to your path.
```shell
export PATH="$PATH:$(pwd)/xpack-riscv-none-elf-gcc-12.2.0-1/bin"
```

## Firmware compilation

1. Clone the project with all its submodules

```shell
git clone --recurse-submodules https://github.com/hydrausb3/HydraDancer.git
```

2. From the top folder, execute

```shell
cmake --toolchain ../cmake/wch-ch56x-toolchain.cmake -DCMAKE_BUILD_TYPE=release -B build .
```

Then

```shell
cmake --build build/
```

And finally

```shell
cmake --install build/
```

3. You should find the `firmware_control_board.bin` compilation artifact in `out/firmware_control_board` and `firmware_emulation_board.bin` in `out/firmware_emulation_board` for the dual-HydraUSB3 configuration. For the Hydradancer dongle, you will find the `firmware_hydradancer.bin` firmware in `out/firmware_hydradancer.bin`.

# Configuring the boards and flashing the firmware(s)

## Physical configuration of the dual-HydraUSB3 configuration

Currently, you need two HydraUSB3 boards connected together via HSPI. You just need to plug the boards together.

- Optional : Add a jumper on PB24 on the top board (might be used by the firmware to determine if it is top or bottom board)

## Permissions

To be able to access the HydraDancer boards and flash them, root privileges may be required, or you can provide them to your regular user, e.g. with the creation of a file `/etc/udev/rules.d/99-hydrausb3.rules` with

```
# UDEV Rules for HydraUSB3 boards https://github.com/hydrausb3, Hydradancer https://github.com/HydraDancer/hydradancer_fw and Facedancer https://github.com/greatscottgadgets/Facedancer
# WinChipHead CH569W Bootloader
SUBSYSTEMS=="usb", ATTRS{idVendor}=="4348", ATTRS{idProduct}=="55e0", MODE="0664", GROUP="plugdev"
# Hydradancer
SUBSYSTEMS=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="27d8", MODE="0664", GROUP="plugdev"
```

and having your user as member of the group `plugdev`.


## Firmware Flashing

### Prerequisites

Clone https://github.com/hydrausb3/wch-ch56x-isp and follow the instructions.

### Flashing for the dual-HydraUSB3 configuration

Set top board in Flash Mode.
With a jumper on P3:

* Put a jumper on the top board (both boards are connected to the same jumper)
* Press & release reset button of the top board

Or with a button or momentary short on P3:

* Press Flash mode button (common to both boards)
* Press & release reset button of the top board
* Release Flash mode button

Warning! You have 10 seconds to flash the board! Note: root privileges may be required, see above.

```shell
wch-ch56x-isp -vf out/firmware_control_board/firmware_control_board.bin
```

Press & release reset button of the top board again to leave Flash Mode. If you used a jumper, don't forget to remove it before resetting the board!

Repeat the same procedure with the bottom board but press the bottom board reset button and flash the proper firmware.

```shell
wch-ch56x-isp -vf out/firmware_emulation_board/firmware_emulation_board.bin
```

### Flashing for the Hydradancer dongle

Set the board in Flash Mode.
With a jumper on P3:

* Put a jumper on the board
* Press & release reset button of the top board

Or with a button or momentary short on P3:

* Press Flash mode button
* Press & release reset button
* Release Flash mode button

Warning! You have 10 seconds to flash the board! Note: root privileges may be required, see above.

```shell
wch-ch56x-isp -vf out/firmware_hydradancer/firmware_hydradancer.bin
```

Press & release reset button of the board again to leave Flash Mode. If you used a jumper, don't forget to remove it before resetting the board!

# More details on compilation

This project uses boards based on the WCH569 RISCV MCU. More documentation can be found on the [HydraUSB3 organization](https://github.com/hydrausb3/).

## Build options

Those options can be set the following way

```shell
cmake --toolchain ../cmake/wch-ch56x-toolchain.cmake -DCMAKE_BUILD_TYPE=release -DOPTION_1=1 -DOPTION_2=1 -B build .
```

- `-DCMAKE_BUILD_TYPE=Debug` use debug optimization
- `-DBUILD_TESTS=1` build the tests

Most warnings will be considered as errors.

## More options

You can set different options to activate more flags, static analysis or the logging system.

Those flags can either be set as build options (but they will apply to all projects) by passing a `-DOPTION=value` to CMake, or by adding a `set(option_name value)` in the project `CMakeLists.txt`.

- `STATIC_ANALYSIS` : activate GCC's built-in static analysers
- `EXTRACFLAGS` : activate -Wconversion and -Wsign-compare

## Logging options

Several logging options can get you infos on different parts of the library/firmwares. By default, no logs are activated so there is no impact on performances.

Some logging filters might activate too much logs for the USB controllers to work properly, a balance has to be found between logging more information and keeping the board working.

In this case, the `LOG` macro will bypass all filters and levels, allowing you to log just what you need.

- Log methods
    - `LOG_OUTPUT=uart`. Logs are written directly to the UART
    - `LOG_OUTPUT=buffer`. Logs are stored in a buffer, and flushed to the UART when calling `LOG_DUMP()`
    - `LOG_OUTPUT=serdes`. Logs are directly sent using Serdes. Might be used to share logs from one board to the other. Not very well tested.
- Log levels
    - `LOG_OUTPUT=x LOG_LEVEL=y`. With y=1(LOG_LEVEL_CRITICAL), y=2(LOG_LEVEL_ERROR), y=3(LOG_LEVEL_WARNING), y=4(LOG_LEVEL_INFO), y=5(LOG_LEVEL_DEBUG). All levels <=y will be displayed.
- Log filters
    - `LOG_OUTPUT=x LOG_FILTER_IDS=a,b,c, ...` You can set any number of filters from the following list 1(LOG_ID_USER), 2(LOG_ID_USB2), 3(LOG_ID_USB3), 4(LOG_ID_HSPI), 5(LOG_ID_SERDES), 6(LOG_ID_INTERRUPT_QUEUE), 7(LOG_ID_RAMX_ALLOC). If `LOG_LEVEL` is also defined, the logs with IDs will only be displayed if they have the right level.