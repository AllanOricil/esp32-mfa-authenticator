#!/bin/bash
set -e

port=""
while [[ $# -gt 0 ]]; do
  case $1 in
    --port)
      port=$2
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

# Check if --port parameter was provided
if [ -z "$port" ]; then
  echo "Error: The --port parameter is required."
  echo "Usage: $0 --port <port_name>"
  echo "Example: $0 --port /dev/cu.usbserial-210"
  exit 1
fi

base_dir=$(pwd)
source $base_dir/scripts/build.sh
cd $base_dir
esptool.py --chip esp32 \
	--port "$port" \
	--baud 115200 write_flash \
	-z 0x0 $base_dir/out/merged_firmware.bin