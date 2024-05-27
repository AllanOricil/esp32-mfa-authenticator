#include <Arduino.h>

#include "ui/ui.h"
#include "constants.h"
#include "utils.hpp"
#include "clock.hpp"
#include "mfa.hpp"
#include "totp-map.h"
#include "file.hpp"
#include "display.hpp"
#include "touch.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"

extern bool isWorkingWithSD;
extern volatile bool processMqttMessage;

void setup()
{
  Serial.begin(115200);

  // SETUP TIME
  init_wifi();
  sync_time();

  // SETUP MFA
  init_sd_card_reader();
  load_mfa_totp_keys();
  generate_totps();

  // SETUP MQTT
  init_mqtt();

  // SETUP SCREEN
  init_display();

  // SETUP UI
  ui_init();

  // SETUP TOUCH
  // NOTE: touch comes after initializing UI because I plan to add a manual calibration screen
  init_touch();
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
