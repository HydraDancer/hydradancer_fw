#!/usr/bin/python3
# Copyright 2023 Quarkslab

"""
Test integrity of SerDes and HSPI devices, the buffer received should be a loop of bytes between 0 and 255.
"""
import sys
import time
import usb.core
import usb.util

ENDP_BURST_SIZE = 1
TOTAL_TIME_NS = 0


def check(array_in):
    for count, val in enumerate(array_in):
        if val != (count % 256):
            return False
    return True


if __name__ == "__main__":
    # find our device
    dev = usb.core.find(idVendor=0x16c0, idProduct=0x27d8)

    if dev is None:
        raise ValueError('Device not found')

    if dev.speed == usb.util.SPEED_HIGH:
        print("USB20 High-speed")
        ENDP_BURST_SIZE = 1
    elif dev.speed == usb.util.SPEED_SUPER:
        ENDP_BURST_SIZE = 4
        print(f"USB30 Superspeed burst {ENDP_BURST_SIZE}")

    # set the active configuration. With no arguments, the first
    # configuration will be the active one
    dev.set_configuration()

    print("Configuration of the device :")

    for cfg in dev:
        sys.stdout.write(str(cfg) + '\n')

    # get an endpoint instance
    cfg = dev.get_active_configuration()
    intf = cfg[(0, 0)]

    ep_in = list(usb.util.find_descriptor(
        intf,
        find_all=True,
        # match the first OUT endpoint
        custom_match=lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_IN))

    assert ep_in is not None

    while 1:
        time.sleep(0.1)
        try:
            TOTAL_TIME_NS = 0
            endp_max_packet_size = ENDP_BURST_SIZE * ep_in[0].wMaxPacketSize
            START = time.time_ns()
            buffer_in = ep_in[0].read(endp_max_packet_size)
            print(buffer_in)
            STOP = time.time_ns()
            if check(buffer_in):
                print("Success !")
                break
            print("Error !")
            break
        except:
            continue
