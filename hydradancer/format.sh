#!/bin/sh

BASE_FOLDER="."
BASE_TEST_FOLDER=./tests
BASE_TOOLS_FOLDER=../tools
HYDRADANCER_FIRMWARES="firmware_control_board firmware_emulation_board"
TEST_FIRMWARES="test_firmware_usb_loopback test_firmware_loopback test_firmware_hspi test_firmware_serdes test_firmware_usb_speedtest test_firmware_unittests test_backend/firmware_emulation_board"
TOOLS_FIRMWARES="firmware_debug_board"
SCRIPTS_FOLDERS="${BASE_TOOLS_FOLDER}/scripts ${BASE_TEST_FOLDER}/scripts"

if [ $# -eq 0 ]
then
    echo "Usage : ./format.sh all|hydradancer|tests|tools"
    exit
fi

format_hydradancer(){
    for firmware in ${HYDRADANCER_FIRMWARES}
    do
        make -C ${BASE_FOLDER}/"$firmware" format || exit
    done
}

format_tests(){
    for firmware in ${TEST_FIRMWARES}
    do
        make -C ${BASE_FOLDER}/"${BASE_TEST_FOLDER}"/"$firmware" format || exit
    done
}

format_tools(){
    for firmware in ${TOOLS_FIRMWARES}
    do
        make -C ${BASE_FOLDER}/"${BASE_TOOLS_FOLDER}"/"$firmware" format || exit
    done
}

format_scripts(){
    for folder in ${SCRIPTS_FOLDERS}
    do
        autopep8 -i -r "$folder"/*.py --verbose || exit
    done

}

if [ "$1" = "all" ];
then
    format_hydradancer;
    format_tests;
    format_tools;
    format_scripts
elif [ "$1" = "hydradancer" ];
then
    format_hydradancer;
elif [ "$1" = "tests" ];
then
    format_tests;
elif [ "$1" = "tools" ];
then
    format_tools;
elif [ "$1" = "scripts" ];
then
    format_scripts;
else
    echo "Please enter a valid command"
    exit;
fi
