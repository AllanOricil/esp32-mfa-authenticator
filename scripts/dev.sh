#!/bin/bash

base_dir=$(pwd)


rm -rf $base_dir/data
mkdir $base_dir/data
cd $base_dir/site
nuxtignore="pages/*.vue
public/tutorial
public/manifest.json
components/tutorial.vue
plugins/esp-web-tools.client.ts"
echo "$nuxtignore" > .nuxtignore
npm ci
npm run generate
cd $base_dir
cp -r $base_dir/site/.output/public/* $base_dir/data
rm -r $base_dir/data/_nuxt/builds
platformio run --target clean
platformio run --environment esp32-cyd
platformio run --target buildfs --environment esp32-cyd


output_dir=$base_dir/out
platformio_build_dir=$base_dir/.pio/build/esp32-cyd
rm -rf $output_dir
mkdir -p $output_dir
cp $platformio_build_dir/bootloader.bin $output_dir/bootloader.bin
cp $platformio_build_dir/partitions.bin $output_dir/partitions.bin
cp $platformio_build_dir/firmware.bin $output_dir/firmware.bin
cp $platformio_build_dir/spiffs.bin $output_dir/spiffs.bin
cp ~/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin $output_dir/boot_app0.bin

cd $output_dir
esptool.py --chip esp32 merge_bin \
	-o merged_firmware.bin \
	--flash_mode dio \
	--flash_freq 40m \
	--flash_size 4MB \
	0x1000 bootloader.bin \
	0x8000 partitions.bin \
	0xe000 boot_app0.bin \
	0x10000 firmware.bin \
	0x290000 spiffs.bin

cd $base_dir
esptool.py --chip esp32 \
	--port /dev/cu.usbserial-220 \
	--baud 115200 write_flash \
	-z 0x0 $base_dir/out/merged_firmware.bin