#!/bin/bash

base_dir=$(pwd)
output_dir=$base_dir/out
platformio_build_dir=$base_dir/.pio/build/esp32-cyd
mkdir -p $output_dir
cp $platformio_build_dir/bootloader.bin $output_dir/bootloader.bin
cp $platformio_build_dir/partitions.bin $output_dir/partitions.bin
cp $platformio_build_dir/firmware.bin $output_dir/firmware.bin
cp $platformio_build_dir/spiffs.bin $output_dir/spiffs.bin
cp ~/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin $output_dir/boot_app0.bin

cd $output_dir
python3 -m esptool --chip esp32 merge_bin \
	-o merged_firmware.bin \
	--flash_mode dio \
	--flash_freq 40m \
	--flash_size 4MB \
	0x1000 bootloader.bin \
	0x8000 partitions.bin \
	0xe000 boot_app0.bin \
	0x10000 firmware.bin \
	0x290000 spiffs.bin