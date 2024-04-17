# Testing

## Running the scripts

First, you need to create a virtual env or reuse one.

Go to the `hydradancer/tests/scripts` folder.

```shell
sudo apt install python3 python3-venv
python3 -m venv venv
```
Activate the virtual environment.

```shell
source venv/bin/activate
```

Install the dependencies.

```shell
pip install requirements.txt
```

Run the scripts.

```shell
python3 script.py
```

or if you need root privileges (if the script does not work, try as follows)

```shell
sudo ./venv/bin/python3 script.py
```
## Tests description

During the development of this project, several tests have been created to check if everything was working as intended.

Different firmwares have been created, along with Python scripts to execute the tests. Those firmwares can be found in the `wch-ch56x-lib` or in `hydradancer/tests`.

Below are the different test cases and how to test them:

### Backend speedtest

* Compile :
  * compile `test_backend/firmware_emulation_board` and `HydraDancer/hydradancer/firmware_control_board` with `make`
  * compile `test_backend/facedancer_replacement_speedtest` with `make` (uses standard gcc)
* Run : flash `test_backend/firmware_emulation_board` to one board and `HydraDancer/hydradancer/firmware_control_board` to the other.

Both boards need to be connected to your computer via USB.

Run the `facedancer_replacement_speedtest` binary using `sudo` if needed. Then, run `test_speedtest.py` or `test_speedtest_one_by_one.py`.

This test effectively replaces Facedancer by a native version : the control firmware is the same, but the emulation board will take care of enumerating the peripheral (to avoid parsing USB control requests).

After launching `facedancer_replacement_speedtest`, a USB peripheral with one endpoint IN and one endpoint OUT will be up and running, and the tests scripts can evaluate the speed of Hydradancer without the Facedancer Python implementation.
