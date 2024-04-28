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
