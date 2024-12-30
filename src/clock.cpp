#include "clock.hpp"

ESP32Time rtc;

static const char *TAG = "clock";

void init_clock()
{
  // NOTE: retrieve time from the NTP server and setting the system time with it
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER_URL);

  // NOTE: wait until time is retrieved
  struct tm time_info;
  time_t now;

  while ((now = time(nullptr)) < 24 * 3600)
  {
    ESP_LOGI(TAG, "waiting for time to be set...");
    delay(500);
  }

  gmtime_r(&now, &time_info);

  // NOTE: set RTC time with the time from the NTP server
  rtc.setTimeStruct(time_info);
  ESP_LOGI(TAG, "time is: %s", rtc.getDateTime().c_str());
}

unsigned long get_elapsed_number_of_time_steps()
{
  return rtc.getEpoch() / TOTP_PERIOD;
}

int get_second()
{
  return rtc.getSecond();
}
