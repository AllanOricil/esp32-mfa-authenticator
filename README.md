# ESP32 MFA TOTP Generator

![Build with PlatformIO](https://img.shields.io/badge/build%20with-PlatformIO-orange?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=) [![CI](https://github.com/AllanOricil/esp32-mfa-totp-generator/actions/workflows/ci.yaml/badge.svg?branch=main)](https://github.com/AllanOricil/esp32-mfa-totp-generator/actions/workflows/ci.yaml)


<a href="https://www.buymeacoffee.com/allanoricil" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" style="height: 40px !important;width: 217px !important;" ></a>



<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/4323be9d-6abe-4873-81f7-417203a3e340" width="500" height="300">

This is personal project that displays MFA TOTP codes. I created it to help me to get TOTPs without interacting with my phone. At the moment, every time I need a new TOTP, I have to:

- find my phone
- hope it is charged
- if it is not, then charge it -> also have to wait a few minutes until it is fully operational to continue
- if it is, unlock it
- find a Virtual MFA App
- unlock it
- find the TOTP for the service I need

Therefore, my plan is to place this MFA device under my monitor, and just look at it when I need to get a new TOTP, instead of doing the aforementioned process. It is going to generate TOTPs for the services I use the most, and have shorter non configurable session times.

Other motives:

- when using my personal phone to get mfa totp codes, I get distracted by notifications and a lot of other things.
- not safe to store secrets on a device that you use when going out. If the device is lost, and there is no security layer to protect unintended access to the virtual MFA App, there is a high chance someone can impersonate your identity.
- secrets are decoupled from the device, and this allows me to easily switch to another one when necessary. I just need to insert the SD card containing my services onto another device flashed with the same code.
- not all services that I use support fido keys or passkeys.
- I wanted to learn how MFA works.
- I wanted to learn how ESP32 works.

https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/166f6ea7-1046-4117-ae22-67991c8e6d8c


https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/6e240518-a35b-4bf0-8a41-ece0dad9efb9


## Board

- `ESP32-2432S028`

## Dev Environment Requirements

- python >= v3.9
- node >= v18.18
- pnpm >= v8.15
- vscode >= v1.87
- platform.io ide vscode extension >= v3.3
- [driver to allow your OS to recognize esp32](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)


> **INFO**: Node.js is required because it runs several development tools used in this project. Among these tools are those that enforce the "conventional commits" standard. This standard is a lightweight convention on top of commit messages, offering an easy set of rules for creating an explicit commit history.


> **INFO**: if platform.io extension does not recognize your board after clicking on `Upload`, `Upload and Monitor` or `Monitor` buttons, it means the driver was not properly setup. In Macos, after installing the driver from Sillicon Labs, I had to restart the system before mac could identify the board.


## Runtime Requirements

- 2.4Ghz WiFi signal with internet connection, in order to sync with NTP
- SD card

## Pre-build Steps

Before building the code, change `WIFI_SSID` and `WIFI_PASSWORD` values in `./src/constants.h` to grant the board access to a network that has access to the internet. This is required because the NTP server is used to set the time in the board.

````c
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
````
> **WARNING**: remember to use a network which has access to the internet, and is isolated from your main network.


## Project Setup

1. Install PlatformIO IDE extension in VSCode.
2. Copy `esp32-2432S028Rv3.json` from `./boards` to `~/.platformio/platforms/ststm32/boards/`.
3. Open this project in a new vscode workspace and wait for Platform.IO to install everything.
4. Connect your board to your computer. If you installed the proper drivers, the next steps should work just fine.
5. Click on the Platform.IO button, in VSCode's sidebar.
6. Then click on `esp32-2432S028Rv3 -> General -> Build` and wait until the build is done.
7. Finally click on `esp32-2432S028Rv3 -> General -> Upload ad Monitor` to flash the code into your board.

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
