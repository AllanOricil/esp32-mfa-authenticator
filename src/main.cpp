#include <Arduino.h>

#include "ui/ui.h"
#include "pin.h"
#include "constants.h"
#include "services.h"
#include "mfa.h"
#include "config.hpp"
#include "utils.hpp"
#include "clock.hpp"
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
  lv_init();
  init_touch_screen(config);
  if (touch_is_calibrated())
  {
    application_state = TOUCH_CALIBRATION_COMPLETE;
  }
  init_wifi(config);
  init_clock();
  init_mfa();
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
    static unsigned long state_change_time = 0;
    lv_scr_load(ui_touch_calibration_screen);
    application_state = TOUCH_CALIBRATION_MIN;
    state_change_time = millis();
    break;

  case TOUCH_CALIBRATION_MIN:
    if (millis() - state_change_time > TOUCH_TIME_TO_CALIBRATE_EACH_POINT)
    {
      touch_calibrate_min();
      ui_touch_calibration_screen_step_2();
      application_state = TOUCH_CALIBRATION_MAX;
      state_change_time = millis();
    }
    break;

  case TOUCH_CALIBRATION_MAX:
    if (millis() - state_change_time > TOUCH_TIME_TO_CALIBRATE_EACH_POINT)
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
    state_change_time = millis();
    break;

  case TOUCH_CALIBRATION_COMPLETE:
    if (millis() - state_change_time > TOUCH_TIME_DISPLAYING_SUCCESS_CALIBRATION_MESSAGE)
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

    unsigned long elapsed_number_of_time_steps = get_elapsed_number_of_time_steps();
    static unsigned long last_step = 0;
    if (elapsed_number_of_time_steps != last_step)
    {
      update_totps();
      ui_totp_screen_update_totp_labels();
      last_step = elapsed_number_of_time_steps;
    }

    int current_second = get_second();
    static unsigned long previous_second = 0;
    if (current_second != previous_second)
    {
      ui_totp_screen_update_totp_countdowns();
      previous_second = current_second;
    }
    break;
  }
  ui_task_handler();
}
