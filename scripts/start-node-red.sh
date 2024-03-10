#!/bin/bash

# Check if Docker is installed
if ! command -v docker &> /dev/null
then
    echo "Docker could not be found. Please install Docker before running this script."
    exit
fi

# Create directories if they do not exist
mkdir -p ~/.node-red
mkdir -p ~/.mosquitto/config
mkdir -p ~/.mosquitto/config/conf.d
mkdir -p ~/.mosquitto/data
mkdir -p ~/.mosquitto/log

# Start Docker services
docker-compose --project-name totp --file docker-compose.totp.yml up --detach 
