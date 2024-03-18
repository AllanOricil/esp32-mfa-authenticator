// System Includes
#include <time.h>

// Library Includes
#include <ESP32Time.h>

// Local Includes
#include "constants.h"

ESP32Time rtc;

void sync_time(){
  // NOTE: retrieve time from the NTP server and setting the system time with it
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER_URL);

  // NOTE: wait until time is retrieved
  struct tm timeinfo;
  time_t now;

  while ((now = time(nullptr)) < 24*3600) {
    Serial.println("Waiting for time to be set...");
    delay(500);
  }

  gmtime_r(&now, &timeinfo);

  // NOTE: set RTC time with the time from the NTP server
  rtc.setTimeStruct(timeinfo); 
  Serial.println(rtc.getDateTime());  
}

