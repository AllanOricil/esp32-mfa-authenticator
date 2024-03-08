# ESP32 MFA TOTP Generator

<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/4323be9d-6abe-4873-81f7-417203a3e340" width="500" height="300">

A pet project that displays MFA TOTP codes. The goal is to allow me to get TOTPs for short living sessions without interacting with my phone. At the moment every time I need a new TOTP, I have to:

- find my phone
- hope it is charged
- if it is not, then charge it -> also have to wait a few minutes until it is fully operational to continue
- if it is, unlock it
- find a Virtual MFA App
- unlock it
- find the TOTP for the service I need

Therefore, my plan is to place this MFA device under my monitor, and just look at it when I need to get a new TOTP, instead of doing the aforementioned process. It is going to generate TOTPs for the services I use the most, and have shorter non configurable session times.

Other reasons:

- when using my personal phone to get mfa totp codes, I get distracted by notifications and a lot of other things.
- not safe to store secrets on a device that you use when going out. If the device is lost, and there is no security layer to protect unintended access to the virtual MFA App, there is a high chance someone can impersonate your identity.
- secrets are decoupled from the device, and this allows me to easily switch to another one when necessary. I just need to insert the SD card containing my services onto another device flashed with the same code.
- I wanted to learn how MFA works.
- I wanted to learn how ESP32 works.

https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/166f6ea7-1046-4117-ae22-67991c8e6d8c


https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/6e240518-a35b-4bf0-8a41-ece0dad9efb9


## Tools

- PlatformIO IDE extension for VSCode.
- Drivers for ESP32 for your OS. 
> **INFO**: for my board and macos, I had to install this [driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) from Silicon Labs.
- In order to configure `ESP32-2432S028` in PlatformIO, follow the instructions found in this [repository](https://github.com/rzeldent/esp32-smartdisplay).

## Requirements

- `ESP32-2432S028`
- 2.4Ghz WiFi signal with internet connection, in order to sync with NTP

## Pre-build Steps

> **WARNING**: before building this code, create a `configuration.h` file in the `src` folder with the following variables.
````c
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
````
> **WARNING**: remember to use a network which has access to the internet, and is isolated from your main network.


## Secrets

Secrets are stored in a file called `secrets.txt`, located in the root of an SD card. It must follow the format shown below:

````bash
service_id,encoded_base_32_secret
````

Each service must be added on a new line. For example:

````bash
aws-1,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
aws-2,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
aws-3,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
````

> **WARNING**: secrets must be unencrypted and based 32 encoded. In the future, Users will have the option to encrypt their secrets, and ask for fingerpint/pin/password before retrieving the current TOTP. The plan is to make this feature configurable per service.

## How to verify if it is working

1. Go to https://totp.danhersam.com/
2. Add your encoded base 32 secret in it, and then compare the TOTP codes it generates with the one you are seeing on the ESP32's screen.


## Roadmap

-  enable the UI to display multiple TOTP codes at once. ✅

	**why?**

	**R:** people often use multiple services that require MFA TOTP codes with high frequency because of their short living sessions.

- read encrypted keys from the sd card, and ask for password/pin/fingerprint during resets in order to decipher them

	**why?** 

	**R:** it is not secure to have unencrypted secrets stored without protection

- enable ESP32 to receive secrets via the local network, or a secure channel.

	**why?** 

	**R:** ease the process of adding new services. With this feature I won't need to insert the SD card on my computer. if there is no SD card on the board, the channel to register new services is going to be closed.
