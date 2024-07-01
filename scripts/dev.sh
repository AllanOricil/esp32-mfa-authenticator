#!/bin/bash

base_dir=$(pwd)
source $base_dir/scripts/build.sh
cd $base_dir
esptool.py --chip esp32 \
	--port /dev/cu.usbserial-220 \
	--baud 115200 write_flash \
	-z 0x0 $base_dir/out/merged_firmware.bin