#!/bin/sh

echo "argv[0]:$0"
echo "argv[1]:$1"


path=`pwd`

buildPath=${path}/build

if [ "$1" = "clean" ]; then
    if [ ! -d $buildPath ];then
        echo "build not exist"
        mkdir build
        return 
    fi
    echo "clean"
    cd ${path}/build && rm -rf *
elif [ "$1" = "build" ]; then
    if [ ! -d $buildPath ];then
        echo "mkdir build"
        mkdir $buildPath
    fi
    cd $buildPath && cmake -DPROJECT=wifi_mcu_develop ..
elif [ "$1" = "all" ]; then
    if [ ! -d $buildPath ];then
        echo "mkdir build"
        mkdir $buildPath
        cd $buildPath && cmake -DPROJECT=wifi_mcu_develop ..
    fi
    # cd $buildPath && make 1>../info.log 2>../warn.log
    cd $buildPath && make
elif [ "$1" = "JFlash" ]; then
    cd $buildPath && make jlink_flash
    elif [ "$1" = "GDBServer" ]; then
    cd $buildPath && make jlink_GDBserver
elif [ "$1" = "make" ]; then
    make PROJECT=wifi_mcu_develop 1>make_info.log 2>make_warn.log
elif [ "$1" = "makeclean" ]; then
    make PROJECT=wifi_mcu_develop clean
fi
