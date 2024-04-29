# ESP32 MFA TOTP Generator

![Build with PlatformIO](https://img.shields.io/badge/build%20with-PlatformIO-orange?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=) [![CI](https://github.com/AllanOricil/esp32-mfa-totp-generator/actions/workflows/ci.yaml/badge.svg?branch=main)](https://github.com/AllanOricil/esp32-mfa-totp-generator/actions/workflows/ci.yaml)

<a href="https://www.buymeacoffee.com/allanoricil" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" style="height: 40px !important;width: 217px !important;" ></a>

<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/bb904daf-c194-4e9f-8948-a0503e94bca1" height="500">

<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/972f3cb1-520e-418c-84a0-9cfe705bc772" height="500">

<img src="https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/83f166a5-1589-4b1f-9d93-2cfba86f4985" height="500">

This is a personal project that creates MFA TOTP codes. I created it to help me to get TOTPs without interacting with my phone. Before creating it, every time I needed a new TOTP, I had to:

- find my phone
- if it is not charged, charge it, and then wait a few minutes until it is fully operational
- if it is charged, unlock it
- find a Virtual MFA App
- unlock it
- find the TOTP for the service I need, which sometimes requires a lot of vertical scrolling.

Other motives:

- when using my personal phone to get mfa totp codes, I get distracted by notifications and a lot of other things.
- not safe to store secrets on a device that you use when going out. If the device is lost, and there is no security layer to protect unintended access to the virtual MFA App, there is a high chance someone can impersonate your identity.
- secrets are decoupled from the device, and this allows me to easily switch to another one when necessary. I just need to insert the SD card containing my services onto another device flashed with the same code.
- not all services that I use support fido keys or passkeys.
- I wanted to learn how MFA works.
- I wanted to learn how ESP32 works.

https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/166f6ea7-1046-4117-ae22-67991c8e6d8c

https://github.com/AllanOricil/esp32-mfa-totp-generator/assets/55927613/6e240518-a35b-4bf0-8a41-ece0dad9efb9

## Parts

- `ESP32-2432S028`
- 3D printed black case
- Acrillic case

> **INFO**: you can buy this board from Aliexpress clicking on any of these affiliate links: [USD](https://s.click.aliexpress.com/e/_mNCBRAA) [BRL](https://s.click.aliexpress.com/e/_mOtZaxM)

> **INFO**: you can buy this acrillic case from Aliexpress clicking on any of these affiliate links: [USD](https://s.click.aliexpress.com/e/_mLYVthc) [BRL](https://s.click.aliexpress.com/e/_m0Tt9wq)

> **INFO** the 3D model for the black case was taking from this [link](https://makerworld.com/en/models/137424#profileId-149549)

### Total Project Cost

| Part                  | Cost     |
| --------------------- | -------- |
| ESP32-2432S028        | 9.25 USD |
| 3D printed black case | 12.7 USD |
| Acrillic case         | 2.5 USD  |

> **INFO**: all prices do not consider expenses with taxes and shipping.

> **INFO**: all prices were taking in February 2024.

## Dev Environment Requirements

- python >= v3.9
- node >= v18.18
- pnpm >= v8.15
- vscode >= v1.87
- platform.io ide vscode extension >= v3.3
- docker >= v25.0
- [driver to allow your OS to recognize esp32](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

> **INFO**: Node.js is required because it runs several development tools used in this project. Among these tools are those that enforce the "conventional commits" standard. This standard is a lightweight convention on top of commit messages, offering an easy set of rules for creating an explicit commit history.

> **INFO**: if platform.io extension does not recognize your board after clicking on `Upload`, `Upload and Monitor` or `Monitor` buttons, it means the driver was not properly setup. In Macos, after installing the driver from Sillicon Labs, I had to restart the system before mac could identify the board.

## Runtime Requirements

- 2.4Ghz WiFi signal with internet connection, in order to sync the board's clock with the NTP server
- SD card

## Pre-build Steps

Before building the code, set the following env variables in your machine:

```bash
export WIFI_SSID=WIFI_SSID
export WIFI_PASSWORD=WIFI_PASSWORD
export PIN_HASH=HMAC_SHA256_PASSWORD_HASH_AS_HEX_STRING
export PIN_KEY=PASSWORD_HASH_KEY
```

- Wi-Fi variables are required because this project uses the NTP server to set its time.
- Pin variables are optional. If both are set, the [pin number screen](https://private-user-images.githubusercontent.com/55927613/326233320-bf9b4940-22e4-42a5-9e29-1b4edcd033eb.jpg?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MTQzNzQzNzMsIm5iZiI6MTcxNDM3NDA3MywicGF0aCI6Ii81NTkyNzYxMy8zMjYyMzMzMjAtYmY5YjQ5NDAtMjJlNC00MmE1LTllMjktMWI0ZWRjZDAzM2ViLmpwZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNDA0MjklMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjQwNDI5VDA3MDExM1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTZiOGRmNzVlZTUwNDIzNmQ0MDYyYTM5ZGU2Y2MwOWJkODIxNTkzYmVlMzdmMjEyMGFiNjYwYzk2MjIzYzAxN2MmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0.ndeMs3_NFAMehUgutSZ2DYogijwZwrvGFUuOhOoGQMI) is shown before your TOTP Codes can be displayed.

> **WARNING**: remember to use a network which has access to the internet, and is isolated from your main network.

> **WARNING**: platform.io vscode extension tasks (build, upload, monitor...) are not using env variables. Therefore, you must open platformio.ini and set `-D WIFI_SSID` and `-D WIFI_PASSWORD` with your values.

> **INFO**: use this [app](https://www.devglan.com/online-tools/hmac-sha256-online) to hash your password. Its hashed output is the value you have to load `PIN_HAS` env variable. Remember to set `PIN_KEY` to the same secret you used to hash your password.

## How to build

1. Install PlatformIO IDE extension in VSCode.
2. Open this project in a new vscode workspace, and wait for Platform.IO to install everything.
3. Connect your board to your computer. If you installed the proper drivers, the next steps should work just fine.
4. Click on the Platform.IO button, in VSCode's sidebar.
5. Then click on `esp32-2432S028Rv3 -> General -> Build` and wait until the build is done.
6. Finally, click on `esp32-2432S028Rv3 -> General -> Upload ad Monitor` to flash the code into your board.

Alternatively, if you prefer using the platform.io cli, follow these steps:

1. run `platformio device list` and annotate the device path of your board.

> **INFO**: You can discover which one belongs to your board by comparing the outputs of this command when your board is connected and not.

2. run `platformio run --environment esp32dev` to build the application
3. upload the code to your board using `platformio run --target upload --upload-port ${DEVICE_PATH} --target monitor --environment esp32dev`.

> **WARNING**: Remember to substitue `${DEVICE_PATH}` with the value you got in step 1.

## How to add TOTP Secrets

The secrets used to compute TOTP codes must be stored in a file called `keys.txt`, and be placed in the root of an SD card. It must follow the format shown below:

```bash
service_id,encoded_base_32_secret
```

Each service must be added on a new line. For example:

```bash
aws-1,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
aws-2,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO
aws-3,DSAJDHHAHASAUDOASNOTREALOADAKLDASAJFPOAIDONTEVENTRYOASFAIPO

```

> **WARNING**: file must end with a new line.

> **WARNING**: secrets must be unencrypted and based 32 encoded. In the future, my plan is to decrypt secrets using a secret stored in the board. With this approach, a stolen SD card won't work on a different board flashed with the same code unless the board has the same key.


## How to verify if TOTP codes are correct

1. Go to https://totp.danhersam.com/
2. Paste/type your encoded base 32 secret in the secret field, and then compare the TOTP code shown with the one you are seeing on the ESP32's screen.

## How to register TOTP Secrets without inserting the SD card into a computer

To enable saving secrets to ESP32 via a local network, this project uses [MQTT](https://mqtt.org/) as the messaging protocol, [Node-red](https://nodered.org/) as the postman (per say) and [Eclipse Mosquito](https://mosquitto.org/) as the MQTT broker. Both services can be started using docker compose using a docker-compose.yaml file located in the root of this project, in order to ease the setup. So, before continuing, install Docker on your computer following the guide found [here](https://www.docker.com/get-started/).

After that, run the following script to start both node-red and the mqtt broker:

```bash
./scripts/start-node-red.sh
```

Open node-red at `http://localhost:1880`, and then load the flow from `./node-red/insert-secret.json`. For now you must manually setup the node input with the secret you want to send to your ESP32, but in the future I plan to have a small app and a chrome extension as clients using the local node-red services to ease the process of storing secrets on the board.

> **WARNING** Make sure to have the following ports free before running `./scripts/start-node-red.sh`: 1880 (node-red), 1883 (eclipse/mosquitto), 9001 (eclipse/mosquito).

You should see the following containers in the docker app.

<img src="./images/docker-compose-totp-service-running.png" width="800">

<img src="./images/docker-node-red-start.png" width="800">

<img src="./images/docker-mosquitto-start.png" width="800">

> **WARNING** remember to assign static ips to the host running the MQTT service, as weel as for the esp32, in your router. This is a suggestion to avoid having to update the `MQTT_SERVER` constant with a new ip every time your router decides to change the ip of your host.

> **WARNING** if your host can't receive messages from other devices on the same network, it could be a firewall problem. Configure the firewall in the host to enable it to receive requests from other devices on your local network.

After all services have initialized, open node-red at `localhost:1880`, and import `./node-red/insert-secret.json` flow. Use this [tutorial](https://nodered.org/docs/user-guide/editor/workspace/import-export) to guide you to import flows into node-red.


## Roadmap

- enable the UI to display multiple TOTP codes at once. ✅

  **why?**

  **R:** people often use multiple services that require MFA TOTP codes with high frequency because of their short living sessions.

- read encrypted keys from the sd card, and ask for password/pin during resets in order to exhibit them ✅

  **why?**

  **R:** it is not secure to have unencrypted secrets stored without protection

- enable ESP32 to receive secrets via a local network, using a secure channel. ✅

  **why?**

  **R:** ease the process of adding new services. With this feature I won't need to insert the SD card on my computer. If there is no SD card on the board, the channel to register new services is going to be closed. I also plan to require fingerprint/pin/password before opening this channel.
