# HydraDancer

Hydradancer provides a faster, USB2 High-Speed capable backend for Facedancer by taking advantage of fast communication protocols such as USB3, SerDes and HSPI.

# Comparison with other Facedancer boards

As shown in the table below, Hydradancer currently supports 5 endpoints other than endpoint 0 in either the IN or OUT directions, with numbers between 1-7. Hydradancer can also emulate USB2 High-speed peripherals. Host mode is currently unsupported, but could be implemented if needed.

|Board|Maximum speed |Number of endpoints (not EP0) |Host mode|
|:---:|:----:|:-:|:-:|
Facedancer21/Raspdancer |USB2 Full-speed |EP1 OUT, EP2 IN, EP3 IN |yes|
GreatFET One |USB2 Full-speed |3 IN / 3 OUT |yes|
**Hydradancer** |USB2 High-speed |6 IN / 6 OUT |no|
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
      <th>Hydradancer High-speed</th>
      <td>3911±151 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>2653±96 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer High-speed (priming)</th>
      <td>3788±194 KB/s</td>
      <td>5%</td>
      <td>499.712KB</td>
      <td>2962±118 KB/s</td>
      <td>4%</td>
      <td>499.712KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer Full-speed (priming)</th>
      <td>369.80±2.46 KB/s</td>
      <td>1%</td>
      <td>49.984KB</td>
      <td>352.35±6.66 KB/s</td>
      <td>2%</td>
      <td>49.984KB</td>
      <td>99.9%</td>
    </tr>
    <tr>
      <th>Hydradancer Full-speed</th>
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
* (unmaintained) the dual-HydraUSB3 : you will need the firmware compiled from `hydradancer/firmware_control_board` and `hydradancer/firmware_emulation_board`.

To build and flash the firmware, see [the build tutorial](BUILD.md). If you don't want to build the firmwares yourself, you can skip the building part by using the [latest release](https://github.com/HydraDancer/hydradancer_fw/releases/latest).

# ... and finally, using Facedancer with Hydradancer !

First, clone Facedancer. While we hope to merge the Hydradancer backend for Facedancer into the [main repository](https://github.com/greatscottgadgets/Facedancer) along with some bug fixes we may have found, the Hydradancer backend is currently in our fork.

For the unmaintained dual-HydraUSB3 firmware, you will need v1.0.0 of our Facedancer fork.

```shell
git clone https://github.com/HydraDancer/Facedancer
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

Then, tell Facedancer to use the Hydradancer backend

```shell
export BACKEND=hydradancer
```

And finally, run one of the examples to check if everything works, this one should make your cursor wiggle.

```shell
python3 ./examples/crazy-mouse.py
```

More information on the different peripherals that can be emulated can be found in `docs/Facedancer.md`

_Note for the dual-HydraUSB3 configuration: you might need to reset both boards after flashing the firmwares (control board first, then emulation board), or if any problem arises when running the scripts._

# Structure of the project

```
hydradancer/
|   ├─  firmware_control_board # firmware for the board connected to Facedancer, dual-HydraUSB3 configuration
|   ├─  firmware_emulation_board # firmware for the board connected to the target USB port, dual-HydraUSB3 configuration
|   ├─  firmware_hydradancer # firmware for the Hydradancer dongle
|   ├─  tests/ # test firmwares to create various USB devices
|   |   ├─ test_backend # Test a Facedancer-like configuration, but without Facedancer. Not up-to-date.
|   |   ├─ native/ # C programs using libusb to interact with the test firmwares
|   |   ├─ scripts/ # Python scripts using pyusb to interact with the test firmwares
tools/
|   ├─  Docker/ # Docker environment for compilation and formatting
```

# Tests

There are no automated tests for now, but that doesn't mean tests are not required.

For now, the tests in hydradancer/tests consist in loop-back devices, to test for integrity and benchmark the speed of the device in different scenarios.

More information about the different scenarios can be found in [docs/Testing.md](docs/Testing.md).

# How to contribute

If you encounter bugs or want to suggest new features, please check the existing issues and create a new issue if necessary.

To contribute code, it's recommended to check existing issues and inform the other contributors that you are working on something by commenting. This way you won't waste your time working on a feature that was not needed or was already in the works.

After that, please create a pull-request and link the related issue.

Your code should follow the [coding style](CODING_STYLE.md) guidelines.
