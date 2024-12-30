# HTTPS Server

The board doesn't have enough memory to handle lvgl UI and an https server with my current implementation. This is the error that happens:

```bash
E (110403) esp-tls-mbedtls: mbedtls_ssl_handshake returned -0x4310
E (110404) esp_https_server: esp_tls_create_server_session failed
```

sources:
https://github.com/espressif/esp-idf/issues/5287
https://gist.github.com/erikcorry/b25bdcacf3e0086f8a2afb688420678e

After enabling only the wifi and the server, the ssl handshake issue was resolved.

```c
void setup()
{
  Serial.begin(115200);
  init_internal_storage();
  init_external_storage();
  // load_services();
  Configuration config = Configuration::load();

  // init_pin(config.security.pin.hash.c_str(), config.security.pin.key.c_str());
  // lv_init();
  // init_touch_screen(config);
  init_wifi(config);
  // init_clock();
  init_manager();
  // init_ui(
  //     config.is_secure(),
  //     config.security.maxNumberOfWrongUnlockAttempts);
}

void loop(){
}
```

I will probably switch to a reverse proxy
