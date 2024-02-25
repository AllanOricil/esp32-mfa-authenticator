#include <Arduino.h>
#include <SD.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <mfa.hpp>
#include "totp-map.h"
#include <WiFi.h>
#include <time.h>
#include "configuration.h"
#include "Base32.h"

#define TF_CS 5

unsigned long lastPrintTime = 0;
unsigned long lastUpdate = 0;
const unsigned long TOTP_PERIOD = 30000;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;  // Replace with your actual timezone offset (in seconds)
const int   daylightOffset_sec = 3600; // Replace with your daylight saving offset (in seconds)

const char* ssid = WIFI_SSID;
const char* pwd = WIFI_PASSWORD;

int setup_complete = 0; 

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

DecodedBase32Secret decode_encoded_base32_secret(const char *secret) {
  Serial.printf("decoding %s\n", secret);

  byte* tempDecoded = NULL;
  Base32 base32;

  int secretLength = strlen(secret);
  byte *secretBytes = new byte[secretLength];
  
  for (int i = 0; i < secretLength; i++) {
    secretBytes[i] = secret[i];
  }
  
  int keyLength = base32.fromBase32(secretBytes, secretLength, (byte*&)tempDecoded);

  Serial.println("--------- start decoded base 32 ---------");
  for (int i = 0; i < keyLength; i++) {
      Serial.print(tempDecoded[i], HEX);
      Serial.print(" ");
  }
  Serial.println();
  Serial.println("--------- end decoded base 32 ---------");

  Serial.printf("key length: %d\n", keyLength);

  DecodedBase32Secret decodedBase32Secret;
  decodedBase32Secret.length = keyLength;
  decodedBase32Secret.value = tempDecoded;

  delete[] secretBytes;

  return decodedBase32Secret; 
}

void init_secrets(){
  Serial.println("decoding secrets");

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
    Serial.println("failed to open /secrets.txt for reading");
    return;
  }

  while (file.available()) {
    Serial.println("reading line");
    String line = file.readStringUntil('\n');
    Serial.println("line read");
    int delimiterIndex = line.indexOf(',');
    
    if (delimiterIndex != -1) {
      Serial.println("retrieving service and secret information");
      String service = line.substring(0, delimiterIndex);
      String encodedBase32Secret = line.substring(delimiterIndex + 1);
      Serial.printf("service: %s\n", service);
      Serial.printf("encoded base 32 secret: %s\n", encodedBase32Secret);
      char* serviceString = strdup(service.c_str());
      char* encodedBase32SecretString = strdup(encodedBase32Secret.c_str());
      upsert_decoded_base32_secret(serviceString, decode_encoded_base32_secret(encodedBase32SecretString));
      Serial.println("service and secret information stored");
    }
  }

  Serial.println("all secrets were decoded");

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

  // UI
  smartdisplay_init();
  auto disp = lv_disp_get_default();
  lv_disp_set_rotation(disp, LV_DISP_ROT_90);
  ui_init();
  
  // SETUP
  init_wifi();
  sync_time();
  print_current_time();
  init_secrets();
  generate_many_totp();
  
  setup_complete = 1;
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

  lv_timer_handler();
}

