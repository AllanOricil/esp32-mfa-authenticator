#!/bin/bash

set -e
BASE_DIR=$(pwd)

# NOTE: validate binary dependencies are installed
if ! command -v openssl &>/dev/null; then
  echo "Error: openssl is required but not installed. Please, refer to https://github.com/openssl/openssl" >&2
  exit 1
fi

if ! command -v yq &>/dev/null; then
  echo "Error: yq is required but not installed. Please, refer to https://github.com/mikefarah/yq" >&2
  exit 1
fi

password=""
salt=""
file=""
while [[ $# -gt 0 ]]; do
  case $1 in
    --password)
      password=$2
      shift 2
      ;;
    --salt)
      salt=$2
      shift 2
      ;;
	--file)
      file=$2
      shift 2
      ;;
    *)
      echo "Unknown option: $1"  >&2
      exit 1
      ;;
  esac
done


if [ -z "$file" ]; then
  echo "Error: The --file parameter is required. It must be a valid file path pointing to services.yml"  >&2
  echo "Usage: $0 --file <file>"  >&2
  echo "Example: $0 --file /Volumes/esp-sd/services.yml"  >&2
  exit 1
fi

key=$(source $BASE_DIR/scripts/generate-key.sh --password "$password" --salt "$salt")

echo "Using key: $key to decrypt services.yml"

yq '.services[] | select(.secret != null) | .secret' "$file" | while IFS= read -r encrypted_secret; do
  echo "Processing secret: $encrypted_secret"
  if [ -z "$encrypted_secret" ]; then
    echo "Error: Encrypted secret is empty or invalid"  >&2
    exit 1
  fi

  decrypted_secret=$(echo "$encrypted_secret" | openssl enc -aes-256-ecb -d -K "$key" -base64 2>/dev/null)
  if [ $? -ne 0 ]; then
    echo "Error: Failed to decrypt secret: $encrypted_secret"  >&2
    exit 1
  fi

  echo "Decrypted secret: $decrypted_secret"

  yq -i "(.services[] | select(.secret == \"$encrypted_secret\") | .secret) = \"$decrypted_secret\"" "$file"
done