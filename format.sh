#!/bin/sh

BASE_FOLDER=$(pwd)
BASE_TEST_FOLDER=./hydradancer/tests
BASE_TOOLS_FOLDER=./tools
LIB_FOLDER=./hydradancer/lib
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
        cd ${BASE_FOLDER}/hydradancer/"$firmware" || exit
        sh format.sh || (cd "${BASE_FOLDER}" && exit)
        cd ${BASE_FOLDER} || exit
    done
}

format_lib(){
    for firmware in ${HYDRADANCER_FIRMWARES}
    do
        cd ${LIB_FOLDER} || exit
        sh format.sh || (cd "${BASE_FOLDER}" && exit)
        cd ${BASE_FOLDER} || exit
    done
}

format_tests(){
    for firmware in ${TEST_FIRMWARES}
    do
        cd ${BASE_FOLDER}/"${BASE_TEST_FOLDER}"/"$firmware" || exit
        sh format.sh || (cd "${BASE_FOLDER}" && exit)
        cd ${BASE_FOLDER} || exit
    done
}

format_tools(){
    for firmware in ${TOOLS_FIRMWARES}
    do
        cd ${BASE_FOLDER}/"${BASE_TOOLS_FOLDER}"/"$firmware" || exit
        sh format.sh || (cd "${BASE_FOLDER}" && exit)
        cd ${BASE_FOLDER} || exit
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
    format_lib;
    format_tests;
    format_tools;
    format_scripts
elif [ "$1" = "hydradancer" ];
then
    format_hydradancer;
elif [ "$1" = "lib" ];
then
    format_lib;
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
