#include <Arduino.h>

#include "pin.h"
#include "ui/ui.h"
#include "constants.h"
#include "totp-map.h"
#include "config.hpp"
#include "utils.hpp"
#include "clock.hpp"
#include "mfa.hpp"
#include "file.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
#include "touch-screen.hpp"

extern bool isWorkingWithSD;
extern volatile bool processMqttMessage;

void setup()
{
  Serial.begin(115200);
  // SETUP SD CARD
  init_sd_card_reader();

  // SETUP CONFIG
  Configuration config = init_configuration();

  // SETUP PIN
  init_pin(config.security.pin.hash.c_str(), config.security.pin.key.c_str());

  // SETUP TIME
  init_wifi(config);
  sync_time();

  // SETUP MFA
  load_mfa_totp_keys();
  generate_totps();

  // SETUP MQTT
  init_mqtt(config);

  // SETUP TOUCH SCREEN
  init_touch_screen(config);

  // SETUP UI
  // TODO: create setup screen with steps for configuring WIFI, MQTT and calibrate touch
  // TODO: encapsulate this logic the UI controller
  bool displayPinScreen = !config.security.pin.hash.isEmpty() && !config.security.pin.key.isEmpty();
  ui_init(displayPinScreen);
}

void loop()
{
  // NOTE: display available free memory
  print_free_memory();

  // NOTE: connect to mqtt broker and subscribe to topics
  connect_to_mqtt();

  // NOTE: ensures totps are generated exactly every 30 seconds. For example: 00:00:00, 00:00:30, 00:01:00, 00:01:30...
  unsigned long now = ((rtc.getMinute() * 60) + rtc.getSecond());
  static unsigned long nextTrigger = 0;
  if (now % TOTP_PERIOD == 0 && now != nextTrigger)
  {
    generate_totps();
    refresh_totp_labels();
    nextTrigger = now;
  }

  // NOTE: ensures the counter is updated on every second, instead of after 1 second
  static unsigned long previousSecond = 0;
  unsigned long currentSecond = rtc.getSecond();
  if (currentSecond != previousSecond)
  {
    refresh_counter_bars();
    previousSecond = currentSecond;
  }

  // NOTE: semaphore to notify that there is a mqtt message waiting to be processed
  if (processMqttMessage)
  {
    processMqttMessage = false;
    // NOTE: semaphore to allow a single processes to work with the sd card
    if (!isWorkingWithSD)
    {
      isWorkingWithSD = true;
      add_new_secret(globalPayload.payload, globalPayload.length);
      isWorkingWithSD = false;
    }
  }

  ui_task_handler();
}
