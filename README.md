# HydraDancer

Hydradancer provides a faster, USB2 High-Speed capable backend for Facedancer by taking advantage of fast communication protocols such as USB3, SerDes and HSPI.

The following examples have been confirmed working:
* hackrf-info.py
* imperative.py
* mass-storage.py
* minimal.py (with test_minimal.py)
* rubber-ducky.py
* template.py
* usbproxy.py : USB Flash Drive in USB2 High-Speed

**NOTE** : current results for the [highly-stressed stress test of Facedancer](https://github.com/greatscottgadgets/facedancer/blob/main/test/test_stress.py) with 20000 tries.

TLDR : the stress test usually fails after a few thousand tries in both Full-Speed and High-Speed. In practice, Hydradancer is usable (see list of devices above) but might fail in highly-stressed situations. Note that we increased the requirements for this stress test (using the highly-stressed one by default and going up to 20000 tries instead of 100).

More about it [here](#stress-test-results).

# Getting started (Hydradancer dongle)

1. To be able to access the HydraDancer boards and flash them, root privileges may be required, or you can provide them to your regular user, e.g. with the creation of a file `/etc/udev/rules.d/99-hydradancer.rules` with

```
# UDEV Rules for HydraUSB3 boards https://github.com/hydrausb3, Hydradancer https://github.com/HydraDancer/hydradancer_fw and Facedancer https://github.com/greatscottgadgets/Facedancer
# WinChipHead CH569W Bootloader
SUBSYSTEMS=="usb", ATTRS{idVendor}=="4348", ATTRS{idProduct}=="55e0", MODE="0664", GROUP="plugdev"
# Hydradancer
SUBSYSTEMS=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="27d8", MODE="0664", GROUP="plugdev"
```

and having your user as member of the group `plugdev`.

2. Flash the firmware to your Hydradancer dongle using the [latest release](https://github.com/HydraDancer/hydradancer_fw/releases/latest) (download `all-firmwares*.zip`) with [wch-ch56-isp](https://github.com/hydrausb3/wch-ch56x-isp/releases/latest)

First

```
Put the Hydradancer dongle in firmware download mode. For that, you need the following buttons :
* reset : button with "RST" next to it
* flash mode : button with "Flash Mode" next to it

You need to hold the flash mode button, press reset (wait 1s then release reset) and then release the flash mode button.
```

Then, launch `wch-ch56x-isp`

```shell
wch-ch56x-isp -vf firmware_hydradancer.bin
```

3. Install Facedancer

Clone our Facedancer work

```shell
https://github.com/HydraDancer/Facedancer
```

Then, reuse your virtual env or create a new one to keep your local Python installation clean

```shell
sudo apt install python3 python3-venv
python3 -m venv venv
```

Activate the venv

```shell
source venv/bin/activate
```

Install Facedancer

```shell
cd Facedancer
pip install --editable .
```

The `--editable` isn't necessary but it allows you to modify Facedancer's files.

4. Test one of the examples

Then, tell Facedancer to use the Hydradancer backend

```shell
export BACKEND=hydradancer
```

And finally, run one of the examples to check if everything works

```shell
python3 ./examples/rubber-ducky.py
```

To test the proxy mode of Facedancer, you might need to use the following line (after editing `usbproxy.py` with the target vid/pid)

```shell
sudo sh -c "env BACKEND=hydradancer ./venv/bin/python3 examples/usbproxy.py"
```

# Comparison with other Facedancer boards

As shown in the table below, Hydradancer currently supports 5 endpoints other than endpoint 0 in either the IN or OUT directions, with numbers between 1-15. Hydradancer can also emulate USB2 High-speed peripherals. Host mode is currently unsupported, but could be implemented if needed.

|Board|Maximum speed |Number of endpoints (not EP0) |Host mode|
|:---:|:----:|:-:|:-:|
Facedancer21/Raspdancer |USB2 Full-speed |EP1 OUT, EP2 IN, EP3 IN |yes|
GreatFET One |USB2 Full-speed |3 IN / 3 OUT |yes|
**Hydradancer** |USB2 High-speed |5 IN / 5 OUT |no|
(Cynthion/LUNA)(coming early 2024) |(USB2 High-speed) |(15 IN / 15 OUT) |(yes)|

<p style="text-align: center "><em>Facedancer backends functionalities</em></p>

As Hydradancer tries to reach higher speeds than Facedancer21 and GreatFET, we benchmarked all the existing boards for comparison. All tests were done using the devices and scripts in the `tests` folder of our [Facedancer fork](https://github.com/HydraDancer/Facedancer). All tests were done using a single libusb transfer, except for GreatFET One which was unresponsive for packets of maximum size (64 bytes at full-speed). In this case, each packet was sent using one libusb transfer, with packets of size 63.

Previous results for Hydradancer used priming, which made it faster. The new versions use NAKs to detect IN transfer requests from the host and calls Facedancer's `handle_nak` method. However devices that support setting buffers in advance ("priming") can still do it by implementing the `handle_buffer_empty` callback, which is called once per "empty buffer" event. As `handle_buffer_empty` was not called by other backends at the moment of the test, only Hydradancer was tested for both methods (answering only after NAKs or priming whenever the buffer is empty).

<table class="table table-striped">
  <thead>
    <tr>
      <th>&nbsp;</th>
      <th>Write average estimate</th>
      <th>Relative write uncertainty</th>
      <th>Write transfer size</th>
      <th>Read average estimate</th>
      <th>Relative read uncertainty</th>
      <th>Read transfer size</th>
      <th>Confidence</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>Hydradancer High-speed (firmware v1.3)</th>
      <td>3911±151 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>2653±96 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer High-speed (priming) (firmware v1.3)</th>
      <td>3788±194 KB/s</td>
      <td>5%</td>
      <td>499.712KB</td>
      <td>2962±118 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer Full-speed (priming) (firmware v1.3)</th>
      <td>369.80±2.46 KB/s</td>
      <td>1%</td>
      <td>49.984KB</td>
      <td>352.35±6.66 KB/s</td>
      <td>2%</td>
      <td>49.984KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer Full-speed (firmware v1.3)</th>
      <td>369.66±4.98 KB/s</td>
      <td>1%</td>
      <td>49.984KB</td>
      <td>266.64±7.32 KB/s</td>
      <td>3%</td>
      <td>49.984KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>GreatFET One Full-speed (one by one) (git-v2021.2.1-64-g2409575 firmware)</th>
      <td>32.42±0.85 KB/s</td>
      <td>3%</td>
      <td>49.959KB</td>
      <td>33.07±1.10 KB/s</td>
      <td>3%</td>
      <td>49.984KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Facedancer21 Full-speed (2014-07-05 firmware)</th>
      <td>0.697±0.000 KB/s</td>
      <td>0%</td>
      <td>9.984KB</td>
      <td>0.682±0.000 KB/s</td>
      <td>0%</td>
      <td>9.984KB</td>
      <td>99.9%</td>
    </tr>
  </tbody>
</table>

<p style="text-align: center "><em>Speedtest results</em></p>

# Building and flashing the firmware(s)

There are two configurations for Hydradancer:

* the Hydradancer dongle : only the firmware from `hydradancer/firmware_hydradancer` is needed.
* (unmaintained) the dual-HydraUSB3 : you will need the firmware compiled from `legacy/hydradancer/firmware_control_board` and `legacy/hydradancer/firmware_emulation_board`.

To build and flash the firmware, see [the build tutorial](BUILD.md). If you don't want to build the firmwares yourself, you can skip the building part by using the [latest release](https://github.com/HydraDancer/hydradancer_fw/releases/latest).

# Structure of the project

```
hydradancer/
|   ├─  firmware_hydradancer # firmware for the Hydradancer dongle
|   ├─  legacy/ # old HydraUSB3 firmwares, unmaintained
|   ├─  tests/ # test firmwares to create various USB devices
|   |   ├─ test_backend # Not up-to-date. Test a Facedancer-like configuration, but without Facedancer.
|   |   ├─ native/ # C programs using libusb to interact with the test firmwares
|   |   ├─ scripts/ # Python scripts using pyusb to interact with the test firmwares
tools/
|   ├─  Docker/ # Docker environment for compilation and formatting
```

# Tests

There are no automated tests for now, but that doesn't mean tests are not required.

For now, the tests in hydradancer/tests consist in loop-back devices, to test for integrity and benchmark the speed of the device in different scenarios.

More information about the different scenarios can be found in [docs/Testing.md](docs/Testing.md).

## Stress-test results

The current Facedancer stress test results are the following.
* USB2 High-Speed
  * bulk IN/ctrl IN : pass
  * bulk OUT/ctrl OUT : fails after a few hundred/thousand tries, rarely reaches 20000
* USB2 Full-Speed
  * bulk IN/ctrl IN : fails after a few hundred/thousand tries, rarely reaches 20000
  * bulk OUT/ctrl OUT : fails after a few hundred/thousand tries, rarely reaches 20000

Currently Hydradancer is usable (see the list of working devices above), however data corruption or timeouts might happen in very stressed conditions. The highly stressed stress test blasts USB transfers of random size and type (control/bulk) and then verifies the integrity of the transfer using USB control transfers.

Solving this issue has proven difficult : it looks random, does not happen immediately (sometimes never). Adding logs or debugging using a USB sniffer can add additional delays and issues. Other architectures have been tried (FreeRTOS, doing all the processing in the interrupt handlers) however they proved to be slower and not more stable.

We implemented a [firmware](https://github.com/hydrausb3/wch-ch56x-lib/tree/main/tests/test_firmware_usb_stress_test) to test the USB2 implementation of `wch-ch56x-lib` with the same stress test and it passes with 100000 tries in both HS and FS. However, Hydradancer's firmware is more complex (more interrupts, USB3 and USB2 at the same time, delays to synchronize with Facedancer).

**Update** We contacted _WCH_ about this issue, however we did not manage to get technical answers. The archive provided to _WCH_ that tries to isolate the issue is available in _docs/wch_questions_wch569.zip_ if you want to take a look.

# How to contribute

If you encounter bugs or want to suggest new features, please check the existing issues and create a new issue if necessary.

To contribute code, it's recommended to check existing issues and inform the other contributors that you are working on something by commenting. This way you won't waste your time working on a feature that was not needed or was already in the works.

After that, please create a pull-request and link the related issue.

Your code should follow the [coding style](CODING_STYLE.md) guidelines.
