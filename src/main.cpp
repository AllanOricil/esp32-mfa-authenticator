#include <Arduino.h>
#include <SD.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <mfa.hpp>
#include "totp-map.h"
#include <WiFi.h>
#include <time.h>
#include "configuration.h"

#define TF_CS 5

unsigned long lastPrintTime = 0;
unsigned long lastUpdate = 0;
const unsigned long TOTP_PERIOD = 30000;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;  // Replace with your actual timezone offset (in seconds)
const int   daylightOffset_sec = 3600; // Replace with your daylight saving offset (in seconds)

const char* ssid = WIFI_SSID;
const char* pwd = WIFI_PASSWORD;

void init_wifi(){
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}


void sync_time(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (time(nullptr) < 24*3600) {
    Serial.println("Waiting for time to be set...");
    delay(1000);
  }
}

void init_secrets(){
  Serial.println("opening file");
  if(!SD.begin(TF_CS)){
    Serial.println("card mount failed");
    return;
  }
  if (!SD.exists("/secrets.txt")) {
    Serial.println("the file does not exist");
    return;
  } 

  File file = SD.open("/secrets.txt");
  if (!file) {
    Serial.println("failed to open file for reading");
    return;
  }

  while (file.available()) {
    Serial.println("reading line");
    String line = file.readStringUntil('\n');
    Serial.println("line read");
    int delimiterIndex = line.indexOf(',');
    
    if (delimiterIndex != -1) {
      Serial.println("line");
      String id = line.substring(0, delimiterIndex);
      String secret = line.substring(delimiterIndex + 1);
      Serial.println("id " + id );
      Serial.println("id " + secret);

      char* idChar = strdup(id.c_str());
      char* secretChar = strdup(secret.c_str());

      upsert_secret(idChar, secretChar);
    }
  }
  print_totps();

  file.close();
}

void print_current_time() {
  struct tm timeinfo;
    
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
    
  Serial.printf("Current time: %04d-%02d-%02d %02d:%02d:%02d\n", (timeinfo.tm_year + 1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}



void setup() {
  Serial.begin(115200);
  init_wifi();
  sync_time();
  print_current_time();
  init_secrets();
  generate_many_totp();
  smartdisplay_init();
  ui_init();
}

void loop() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  unsigned long now = ((timeinfo.tm_min * 60) + timeinfo.tm_sec) * 1000; // Convert current time to milliseconds
  static unsigned long nextTrigger = 0; // Next trigger timestamp

  // Check if we can trigger now or set new trigger 
  if (now >= nextTrigger) {
    Serial.println("generating totp from file");
    generate_many_totp();

    // Calculate next TOTP generation moment
    nextTrigger = ((now / TOTP_PERIOD) + 1) * TOTP_PERIOD;
  }

  if (now - lastPrintTime >= 5000) {
    print_current_time();
    lastPrintTime = now;
  }

  lv_timer_handler();
}

