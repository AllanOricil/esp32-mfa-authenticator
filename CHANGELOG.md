# [0.19.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.18.0...v0.19.0) (2025-01-04)


### Features

* there can exist up to 100 services divided between 10 groups ([981f14c](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/981f14c8ae3e623d8735208c4cc5ed7bca4f2f01))

# [0.18.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.17.0...v0.18.0) (2025-01-02)


### Bug Fixes

* ensure resource routes validate session before providing data ([bedc219](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/bedc219b5f23e297ad057ef94f351e95afb66ce4))
* improve auth api ([df985c1](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/df985c11a482effc949ea63a48f5c576e945d84a))
* invalid password ([ef09c45](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/ef09c45bbe81704bb73689c11f3be43c7d07188c))
* **manager:** error while validating session ([f7ddbba](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/f7ddbbaebb753a8a71dbbf141716734c49bdbd58))
* **manager:** page not loading because assets are missing ([af81775](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/af81775c1973b85db1d53854eaa253b566d39095))
* **manager:** re-auth during hard refreshes while session cookie ([13e8cea](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/13e8ceabb4b4aba6b69d2f6de3cef586ad866be9))
* **manager:** unauthorized due to missing session ([9a2b3c4](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/9a2b3c46d7e8640befc365fecda328344f40eee1))


### Features

* add username/password auth route in the manager server ([e38fe39](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/e38fe39147ab3196364a5b143ce4e6f7ac4a6cc9))
* enable manager only if username, password and key are set in config.yml ([6e299be](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/6e299be6c3c1e0bf0509fb97545670dcc190da60))
* **manager:** add authentication with username and password ([cc01402](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/cc014029fddbf44ab7f08af0a63dbb5d7547c8a0))
* **manager:** add fetchServices and updateServices to esp32-mfa-authenticator client ([c615cb3](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/c615cb3116aec5c6ac3f91b8970d7a6a23902c50))
* **manager:** add first iteration of the icon ([0e92fc6](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/0e92fc6a93994a95b79b8b9362e8ff2e05dd02c3))
* **manager:** fetch services from server ([2b51738](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/2b5173886dc32564b6153bcf05133e786df9f626))

# [0.17.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.16.0...v0.17.0) (2024-12-30)


### Features

* use esp_log.h as the logger library ([09f4ac3](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/09f4ac30644149d520cfb9965ecd31fa4ca6c9a9))

# [0.16.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.15.0...v0.16.0) (2024-12-27)


### Bug Fixes

* align pin screen theme with totp screen for consistency ([f2c59b5](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/f2c59b51e4bddb5efcc805b34affe06104474fcb))


### Features

* group index is rendered when a group does not have services ([cd945c3](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/cd945c3c9e83af9ae1a9e865eb1a78456d9a1e23))

# [0.15.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.14.0...v0.15.0) (2024-12-26)


### Bug Fixes

* ensure group default to 0 when not in bounds ([630f406](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/630f406e0cded11ac004b60712d06908482e2ca7))


### Features

* increase MAX_NUMBER_OF_GROUPS to 10 ([c440976](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/c44097698aa72f91d42e89b8e8d2dd5b963b2655))

# [0.14.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.13.1...v0.14.0) (2024-12-26)


### Features

* enable grouping services by group ([6d290af](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/6d290afc429f61b9274dc8fd17faad007cfee5de))

## [0.13.1](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.13.0...v0.13.1) (2024-12-22)


### Bug Fixes

* destroy touch config screen when no longer needed to release mem ([e01f8de](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/e01f8de92122669768c12217466f9205008f7709))
* min==max delta due to calibration values being all 0 ([ebf7deb](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/ebf7debe9053b95f746b9d4eb5883b2fa135bf46))

# [0.13.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.12.0...v0.13.0) (2024-12-22)


### Features

* add screen to configure touch sensor ([75485d3](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/75485d3fb14257d6f925de2422cf1fe0d7a92410))

# [0.12.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.11.0...v0.12.0) (2024-12-07)


### Features

* display PIN screen immediately upon screen wake-up ([e548745](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/e548745726adc1a1f8dac2cbf39fb3cd10ebe775))

# [0.11.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.10.1...v0.11.0) (2024-07-29)


### Features

* lock board after N failed unlock attempts ([d30f1ab](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/d30f1abf5b5c436c76480a2dd75a4bddfe7055dc))

## [0.10.1](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.10.0...v0.10.1) (2024-07-07)


### Bug Fixes

* **manager:** remove exception added for testing error when submiting the settings form ([f6e93d8](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/f6e93d8e1b839580afb8032f8ce1f9669a6d88c0))
* **site:** remove manager default layout from the presentation site ([84e1fc3](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/84e1fc3accdd5da74419e6f635fb1a5709b329ce))

# [0.10.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.9.2...v0.10.0) (2024-07-01)


### Bug Fixes

* **esp32:** fix yaml serialization ([57d610f](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/57d610fecf96a2306554272a1e5267089e8b4985))
* **manager:** fix routing issue caused by the spiffs.bin ([73e04a0](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/73e04a00aa2de2a8f458c88f86edb28dfeace621))


### Features

* **esp32:** add server routes for fetching and updating the config file directly from the board ([003b05c](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/003b05ca00af35861f88de4e083bacb67a7654ca))
* **manager:** display a toast message when submiting the settings form ([58bce30](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/58bce305b097f03f002a2f729af1fa76839e89e6))
* **manager:** expose static site in the board so that it can be managed ([2dda6f9](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/2dda6f917df0961287142ddcf7a43e70993af9b4))
* **manager:** initial implementation for the settings form ([7ba22bc](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/7ba22bc8b0720c18eb58ed1785866681c09b60f3))
* **manager:** setup navbar and home page ([57c6699](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/57c66998500bf394927f70c0785ecec076ba7ef9))

## [0.9.2](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.9.1...v0.9.2) (2024-06-24)


### Bug Fixes

* prepend all tutorial image paths with the repo's slug because nuxt github-actions preset doesn't do it ([857d9eb](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/857d9eb1eb0406c2d274a0d3da00eb20f1819e83))

## [0.9.1](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.9.0...v0.9.1) (2024-06-24)


### Bug Fixes

* **site:** _nuxt assets can now be fetched ([b4666ff](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/b4666ff857c15b091a425aaa476413a5f35757ef))

# [0.9.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.8.0...v0.9.0) (2024-06-23)


### Features

* add a site that enables users to easily flash their ESP32-CYD board ([a72bb74](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/a72bb7475d4cb4a484cde525bee650a0c7b90e59))

# [0.8.0](https://github.com/AllanOricil/esp32-mfa-authenticator/compare/v0.7.0...v0.8.0) (2024-06-09)


### Features

* display turns off after n seconds ([c072a9a](https://github.com/AllanOricil/esp32-mfa-authenticator/commit/c072a9a878c118ba7c92a04af758e8d191f0e603))

# [0.7.0](https://github.com/AllanOricil/esp32-mfa-totp-generator/compare/v0.6.0...v0.7.0) (2024-06-02)


### Bug Fixes

* pin screen buttons are clickable again ([76ba5a8](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/76ba5a8a1a97753f94644b283af822e5d3dd0cae))
* pin validation no longer throws a runtime exception ([72a43d3](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/72a43d3a52358411b1eb7382318252c762935910))
* src/main.cpp:30: undefined reference to init_pin(char const*, char const*) ([1b0e268](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/1b0e268f49236efddcc54c6f3913a247c297bd69))


### Features

* read user specific values from .yml instead of env variables set during build ([6806dac](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/6806dacced68640f1638c6f51cc42bfaa49ed879))

# [0.6.0](https://github.com/AllanOricil/esp32-mfa-totp-generator/compare/v0.5.0...v0.6.0) (2024-05-05)


### Bug Fixes

* not having an mqtt server running will no longer break the runtime ([4c7f24f](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/4c7f24f0ce82516db7ae7b8ab098ee24c851ba4f))


### Features

* add MQTT configuration options to build flags ([2e21013](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/2e21013c899243ae5a84c31d8a8f2b047c36b09b))

# [0.5.0](https://github.com/AllanOricil/esp32-mfa-totp-generator/compare/v0.4.0...v0.5.0) (2024-04-28)


### Bug Fixes

* double touch interval was reduced to 100ms to feel more natural ([ff05ee3](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/ff05ee3a7e81b10b891930f6eb5ece1386c31033))
* ensures the pin screen appears only if both PIN_HASH and PIN_KEY are set as env variables ([ab1f438](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/ab1f43893d1c9d504580d40a5652906092c80628))


### Features

* add pin validation using SHA256 hash with salt ([6a489f8](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/6a489f86a2b4aba25549996a2d3ad07eec70a520))
* add screen where user can type pin number ([e5a921e](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/e5a921e45e35f84b5b006e902f7b7001aef545eb))
* finish initial pin code implementation ([c740ff1](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/c740ff1667d9df97f7c7dc67e257db40edad0d39))

# [0.4.0](https://github.com/AllanOricil/esp32-mfa-totp-generator/compare/v0.3.0...v0.4.0) (2024-03-20)


### Features

* screen can alternate from on and off states double touching the display ([efaffd0](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/efaffd03e2ab1211731ea5c8058e6e0d977f66aa))

# [0.3.0](https://github.com/AllanOricil/esp32-mfa-totp-generator/compare/v0.2.3...v0.3.0) (2024-03-18)


### Features

* enable writting secrets to the sd card using node-red and mqtt protocol ([a01921d](https://github.com/AllanOricil/esp32-mfa-totp-generator/commit/a01921df02dc1df5834d3abd34b93c4ba93eea71))
