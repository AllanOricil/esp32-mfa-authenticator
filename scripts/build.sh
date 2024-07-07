#!/bin/bash

generate_random_number() {
    local length="$1"
    local num=""
    for ((i=0; i<length; i++)); do
        num="$num$(( RANDOM % 10 ))"
    done
    echo "$num"
}

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

# NOTE: during github release created by github actions, if tag name exists use it, otherwise generate a random number that works with spiffs max path size
# NOTE: without the changes below, client routing won't work because nuxt breaks when it cant load its meta files
# NOTE: these changes address this issue with SPIFFS https://techoverflow.net/2022/08/07/how-to-fix-spiffs_write-error-10010-unknown/
if [[ "$USE_RELEASE_TAG" == "true" ]]; then
	echo "Tag name is set to $GITHUB_REF_NAME"
	export NUXT_BUILD_ID=$GITHUB_REF_NAME
else
    echo "Not a release event. Using a random number for NUXT_BUILD_ID"
    # Generate a random number of 7 characters
    export NUXT_BUILD_ID=$(generate_random_number 7)
fi

npm ci
npm run generate
cd $base_dir
cp -r $base_dir/site/.output/public/* $base_dir/data


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