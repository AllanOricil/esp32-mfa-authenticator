# ESP32 MFA TOTP Generator

<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/5c545e94-9180-409d-9a7f-115982a0db48" width="500" height="300">

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

- when using my personal phone to get mfa totp codes, I can get distracted by notifications and a lot of other things.
- not safe to store secrets on a device that you use when going out. If the device is lost, and there is no security layer to protect unintended access to the virtual MFA App, there is a high chance someone can impersonate your identity.
- secrets are decoupled from the device, and this allows me to easily switch to another one when necessary. I just need to move add my SD card on another device which has the same code.
- I wanted to learn how MFA works.
- I wanted to learn how ESP32 works.

https://github.com/AllanOricil/esp32-mfa-totp-viewer/assets/55927613/c5d344d6-c120-42e2-b779-4a32a116a47c


https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/f66121c8-e4f7-42ef-8708-03f6eb5fed22


## Tools

- PlatformIO IDE extension for VSCode.
- Drivers for ESP32 for your OS. For my board and macos, I had to install this [driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) from Silicon Labs.
- In order to work with `ESP32-2432S028` in PlatformIO, you must install these [boards](https://github.com/rzeldent/esp32-smartdisplay).

## Requirements

- `ESP32-2432S028`
- 2.4Ghz WiFi signal with internet connection, in order to sync with NTP

## Pre-build Steps

> **WARNING**: before building this code, create a `configuration.h` file in the `src` folder with the following variables.
````c
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
````
> **WARNING**: change `WIFI_SSID` and `WIFI_PASSWORD` with one which you esp32 can reach out.


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

## How to verify if it is working

1. Go to https://totp.danhersam.com/
2. Add your encoded base 32 secret in it, and then compare the TOTP codes it generates with the one you are seeing on the ESP32's screen.


## Roadmap

- ✅ enable the UI to display multiple TOTP codes at once. 

	**why?**

	**R:** People often use multiple services that require MFA TOTP codes with high frequency because of their short living sessions.

- encrypt keys and ask for password during resets to decrypt them

	**why?** 

	**R:** it is not secure to have secrets stored without protection

- enable ESP32 to receive secrets via the local network.

	**why?** 

	**R:** In order to avoid having to umplug the SD card and pluging it on my computer every time I have to register a new MFA secret.
