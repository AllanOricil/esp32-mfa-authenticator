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
#include "touch-screen.hpp"
#include "manager.hpp"

enum ApplicationState
{
  TOUCH_CALIBRATION_START,
  TOUCH_CALIBRATION_MIN,
  TOUCH_CALIBRATION_MAX,
  TOUCH_CALIBRATION_UPDATE,
  TOUCH_CALIBRATION_COMPLETE,
  TOTPS_UPDATE
};

ApplicationState application_state = TOUCH_CALIBRATION_START;

void setup()
{
  Serial.begin(115200);

  init_sd_card_reader();

  Configuration config = Configuration::load();
  init_pin(config.security.pin.hash.c_str(), config.security.pin.key.c_str());

  init_touch_screen(config);

  if (touch_is_calibrated())
  {
    application_state = TOUCH_CALIBRATION_COMPLETE;
  }

  init_wifi(config);
  sync_time();
  load_mfa_totp_keys();
  generate_totps();
  init_manager();

  init_ui(
      config.is_secure(),
      config.security.maxNumberOfWrongUnlockAttempts);
}

void loop()
{
  // NOTE: display available free memory
  print_free_memory();

  switch (application_state)
  {
  case TOUCH_CALIBRATION_START:
    static unsigned long lastStateChangeTime = 0;
    lv_scr_load(ui_touch_calibration_screen);
    application_state = TOUCH_CALIBRATION_MIN;
    lastStateChangeTime = millis();
    break;

  case TOUCH_CALIBRATION_MIN:
    if (millis() - lastStateChangeTime > 5000)
    {
      touch_calibrate_min();
      ui_touch_calibration_screen_step_2();
      application_state = TOUCH_CALIBRATION_MAX;
      lastStateChangeTime = millis();
    }
    break;

  case TOUCH_CALIBRATION_MAX:
    if (millis() - lastStateChangeTime > 5000)
    {
      touch_calibrate_max();
      application_state = TOUCH_CALIBRATION_UPDATE;
    }
    break;

  case TOUCH_CALIBRATION_UPDATE:
    touch_save_calibration();
    touch_register();
    touch_set_calibrated();
    ui_touch_calibration_screen_step_3();
    application_state = TOUCH_CALIBRATION_COMPLETE;
    lastStateChangeTime = millis();
    break;

  case TOUCH_CALIBRATION_COMPLETE:
    if (millis() - lastStateChangeTime > 2000)
    {
      lv_obj_clean(lv_scr_act());
      load_first_screen();
      reset_display_off_timer();
      ui_touch_calibration_screen_destroy();
      application_state = TOTPS_UPDATE;
    }
    break;
  case TOTPS_UPDATE:
    display_timeout_handler();

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
      refresh_totp_countdowns();
      previousSecond = currentSecond;
    }
    break;
  }

  ui_task_handler();
}
