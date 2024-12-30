#!/bin/bash

KEY_FILE="key.pem"
CERT_FILE="cert.pem"
SUBJECT="/C=US/ST=State/L=City/O=Organization/CN=ESP32"
DAYS_VALID=365

if ! command -v openssl &> /dev/null; then
    echo "Error: OpenSSL is not installed. Please install it and try again."
    exit 1
fi

openssl genpkey -algorithm RSA -out "$KEY_FILE" -pkeyopt rsa_keygen_bits:2048
if [ $? -ne 0 ]; then
    echo "Error: Failed to generate private key."
    exit 1
fi

echo "Private key saved to $KEY_FILE."

openssl req -new -x509 -key "$KEY_FILE" -out "$CERT_FILE" -days "$DAYS_VALID" -subj "$SUBJECT"
if [ $? -ne 0 ]; then
    echo "Error: Failed to generate self-signed certificate."
    exit 1
fi

echo "Certificate saved to $CERT_FILE."

echo "Key and certificate generation completed successfully."
