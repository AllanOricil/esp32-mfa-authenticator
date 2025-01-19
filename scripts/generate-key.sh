#!/bin/bash

set -e

# TODO: make this random value between 10K and 100K, and store it in config.yml
ITERATIONS=10000
KEY_SIZE=32

# NOTE: validate binary dependencies are installed
if ! command -v openssl &>/dev/null; then
  echo "Error: openssl is required but not installed. Please, refer to https://github.com/openssl/openssl" >&2
  exit 1
fi

password=""
salt=""
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
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

if [ -z "$password" ]; then
  echo "Error: The --password parameter is required." >&2
  echo "Usage: $0 --password <password>" >&2
  echo "Example: $0 --password qw" >&2
  exit 1
fi

if [ -z "$salt" ]; then
  echo "Error: The --salt parameter is required. It must be Base32 encoded" >&2
  echo "Usage: $0 --salt <salt>" >&2
  echo "Example: $0 --salt KXOFAZCLCES66ZW22ZWQVE2YMY======" >&2
  exit 1
fi

decoded_salt=$(echo "$salt" | base32 --decode 2>/dev/null)
if [ -z "$decoded_salt" ]; then
  echo "Error: Failed to decode the Base32-encoded salt." >&2
  exit 1
fi

key=$(openssl kdf \
  -keylen $KEY_SIZE \
  -kdfopt digest:SHA256 \
  -kdfopt pass:$password \
  -kdfopt salt:"$decoded_salt" \
  -kdfopt iter:$ITERATIONS PBKDF2 2>/dev/null | tr -d ':')

if [ -z "$key" ]; then
  echo "Error: Failed to generate the encryption key." >&2
  exit 1
fi

echo "$key"
