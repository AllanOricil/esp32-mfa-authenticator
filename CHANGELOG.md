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
