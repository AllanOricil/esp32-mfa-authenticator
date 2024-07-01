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
npm ci
npm run generate
cd $base_dir
cp -r $base_dir/site/.output/public/* $base_dir/data

# NOTE: without the changes below, client routing won't work.
# NOTE: these changes address this issue with SPIFFS https://techoverflow.net/2022/08/07/how-to-fix-spiffs_write-error-10010-unknown/
# NOTE: Because "/_nuxt/builds/meta/{RANDOM_NUMBER}.json" must be 32 characters long including terminating character, the random number can only be 7 characters long (7+1+24)
nuxt_build_id=$(generate_random_number 7)
mv $base_dir/data/_nuxt/builds/meta/*.json $base_dir/data/_nuxt/builds/meta/$nuxt_build_id.json
jq --arg new_id "$nuxt_build_id" '.id = $new_id' $base_dir/data/_nuxt/builds/meta/*.json > tmp.$$.json && mv tmp.$$.json $base_dir/data/_nuxt/builds/meta/$nuxt_build_id.json
jq --arg new_id "$nuxt_build_id" '.id = $new_id' $base_dir/data/_nuxt/builds/latest.json > tmp.$$.json && mv tmp.$$.json $base_dir/data/_nuxt/builds/latest.json
sed "s/buildId:\"[^\"]*\"/buildId:\"$nuxt_build_id\"/" $base_dir/data/index.html > tmp.$$.index.html && mv tmp.$$.index.html $base_dir/data/index.html
sed "s/buildId:\"[^\"]*\"/buildId:\"$nuxt_build_id\"/" $base_dir/data/200.html > tmp.$$.200.html && mv tmp.$$.200.html $base_dir/data/200.html
sed "s/buildId:\"[^\"]*\"/buildId:\"$nuxt_build_id\"/" $base_dir/data/404.html > tmp.$$.404.html && mv tmp.$$.404.html $base_dir/data/404.html

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