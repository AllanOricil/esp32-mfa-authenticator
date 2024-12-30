#!/bin/bash
set -e

port=""
env=""
while [[ $# -gt 0 ]]; do
  case $1 in
    --port)
      port=$2
      shift 2
      ;;
    --env)
      env=$2
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

if [ -z "$port" ]; then
  echo "Error: The --port parameter is required."
  echo "Usage: $0 --port <port_name>"
  echo "Example: $0 --port /dev/cu.usbserial-210"
  exit 1
fi

if [[ "$env" != "dev" && "$env" != "prod" ]]; then
  echo "Error: Invalid environment specified. Allowed values are 'dev' or 'prod'."
  exit 1
fi

base_dir=$(pwd)
source $base_dir/scripts/build.sh --env "$env"
cd $base_dir
esptool.py --chip esp32 \
	--port "$port" \
	--baud 115200 write_flash \
	-z 0x0 $base_dir/out/merged_firmware.bin