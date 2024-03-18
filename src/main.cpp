// System Includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SD.h>
#include <WiFi.h>
#include <time.h>

// Library Includes
#include <ui/ui.h>
#include <mfa.hpp>
#include <PubSubClient.h>

// Local Includes
#include "totp-map.h"
#include "Base32.h"
#include "ESP32Time.h"
#include "constants.h"
#include "file.hpp"
#include "tft-touch.h"

#ifndef WIFI_SSID
#define WIFI_SSID (char*)"CHOCOLATE"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD (char*)"CHOCOLATE"
#endif

ESP32Time rtc;

WiFiClient espClient;
PubSubClient client(espClient);

char mqttServer[] = MQTT_SERVER;
volatile bool processMqttMessage = false;
bool isWorkingWithSD = false;

struct PayloadData {
  byte payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

volatile PayloadData globalPayload = {{0}, 0};


void init_sd_card_reader(){
  if(!SD.begin(TF_CS)){
    Serial.println("sd card mount failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
      Serial.println("MMC");
  } else if (cardType == CARD_SD) {
      Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  Serial.printf("SD Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void add_new_secret(volatile byte* payload, unsigned int length){
  DynamicJsonDocument doc(1024);
  char _payload[length+1];
  memcpy(_payload, const_cast<byte*>(payload), length);
  _payload[length] = '\0';
  auto error = deserializeJson(doc, _payload);
  if (error) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
    }

  if (doc.containsKey("service") && doc.containsKey("key")) {
    const char *service = doc["service"];
    const char *key = doc["key"];

    // If you still get empty, print them out to debug
    Serial.print("Service:");
    Serial.println(service);

    Serial.print("Key:");
    Serial.println(key);

    if(!service || strlen(service)>60) {
      Serial.println("Invalid or missing service");
      return;
    }
    
    if(!key || (strlen(key) != 16 && strlen(key) != 32 && strlen(key) != 64)) {
        Serial.println("Invalid or missing key");
        return;
    }

    char newSecret[MAX_SERVICE_NAME_LENGTH + MAX_TOTP_LENGTH];
    sprintf(newSecret, "%s,%s\n", service, key);

    append_file(SD, KEYS_FILE_PATH, newSecret);
  } else {
    Serial.println("Keys not found in JSON.");
  }
}

void print_mqtt_topic_message(char* topic, byte* payload, unsigned int length) {
    // Print topic
    Serial.print("MQTT Message arrived on topic: ");
    Serial.println(topic);
    
    // Print payload
    Serial.println("With Payload:");
    char message[length+1];
    memcpy(message, payload, length);
    message[length] = '\0'; // Null-terminate the array to create a valid C-string

    Serial.println(message);
}

void on_mqtt_message_received(char* topic, byte* payload, unsigned int length) {
  print_mqtt_topic_message(topic, payload, length);
  if(length <= MQTT_MAX_PAYLOAD_SIZE) {
    processMqttMessage = true;
    byte nonVolatilePayload[MQTT_MAX_PAYLOAD_SIZE];
    memcpy(nonVolatilePayload, payload, length);
    for(unsigned int i = 0; i < length; i++) {
        globalPayload.payload[i] = nonVolatilePayload[i];
    }
    globalPayload.length = length;
  }
}

void connect_to_mqtt() {
  Serial.print("Connecting to MQTT...");
  if (client.connect("esp32-mfa-totp-generator")) {
    Serial.println("connected");
    client.subscribe(MQTT_WRITE_NEW_SECRET_TOPIC);
    Serial.println("subscribed to esp32-totp-write-new-secret topic");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.printf("will try again in %d ms\n", MQTT_RECONNECT_INTERVAL);
  }
}

void init_wifi(){
  if(WIFI_SSID == NULL || WIFI_PASSWORD == NULL){
    Serial.printf("Failed to get WiFi details from environment variables");
    return; // Or handle error appropriately
  }

  Serial.printf("Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());
}

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

  gmtime_r(&now, &timeinfo); // initialize timeinfo

  // NOTE: set RTC time with the time from the NTP server
  rtc.setTimeStruct(timeinfo); 
  Serial.println(rtc.getDateTime());  
}

DecodedBase32Secret decode_encoded_base32_secret(char *secret) {
  Serial.printf("decoding %s\n", secret);

  byte* tempDecoded = NULL;
  Base32 base32;

  int secretLength = strlen(secret);
  byte *secretBytes = new byte[secretLength];
  
  for (int i = 0; i < secretLength; i++) {
    secretBytes[i] = secret[i];
  }
  
  int keyLength = base32.fromBase32(secretBytes, secretLength, (byte*&)tempDecoded);

  Serial.println("decoded base 32 secret");
  for (int i = 0; i < keyLength; i++) {
      Serial.print(tempDecoded[i], HEX);
      Serial.print(" ");
  }
  Serial.println();
  Serial.printf("length: %d\n", keyLength);

  DecodedBase32Secret decodedBase32Secret;
  decodedBase32Secret.length = keyLength;
  decodedBase32Secret.value = tempDecoded;

  delete[] secretBytes;

  return decodedBase32Secret; 
}

void load_mfa_totp_keys(){
  Serial.println("fetching /keys.txt");

  if (!SD.exists(KEYS_FILE_PATH)) {
    Serial.println("keys.txt file does not exist");
    return;
  }

  File file = SD.open(KEYS_FILE_PATH, FILE_READ);
  if (!file) {
    Serial.println("failed to open /keys.txt for reading");
    return;
  }

  Serial.println("decoding secrets");
  while (file.available()) {
    String line = file.readStringUntil('\n');
    Serial.printf("processing line: %s\n", line.c_str());
    int delimiterIndex = line.indexOf(',');
    if (delimiterIndex != -1) {
      // NOTE: converts const char* to char* because of upsert_decoded_base32_secret
      char* service = strdup(line.substring(0, delimiterIndex).c_str());
      char* encodedBase32Secret = strdup(line.substring(delimiterIndex + 1).c_str());
      Serial.printf("service: %s\n", service);
      Serial.printf("encoded base 32 secret: %s\n", encodedBase32Secret);
      upsert_decoded_base32_secret(service, decode_encoded_base32_secret(encodedBase32Secret));
      Serial.println("service and secret information stored\n");
    }
  }
  Serial.println("all secrets were decoded");
  file.flush();
  file.close();
  Serial.println("file closed after reading secrets");
}

void init_mqtt(){
  client.setServer(mqttServer, MQTT_PORT);
  client.setCallback(on_mqtt_message_received);
}

void setup() {
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

  // SETUP UI
  init_display_and_touch();
  ui_init();
}

long lastPrint = 0;  
unsigned long printInterval = 5000;

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - lastPrint >= printInterval) {
    lastPrint = currentMillis;
    Serial.print("Free Memory: ");
    Serial.println(xPortGetFreeHeapSize());
  }


  // NOTE: connect to MQTT topics every 5000 ms
  static unsigned long lastMqttReconnectAttempt = 0;
  if (!client.connected() && (currentMillis - lastMqttReconnectAttempt > MQTT_RECONNECT_INTERVAL)) {
    lastMqttReconnectAttempt = currentMillis;
    connect_to_mqtt();
  } else {
    client.loop();
  }

  // NOTE: ensures totps are generated exactly every 30 seconds. For example: 00:00:00, 00:00:30, 00:01:00, 00:01:30...
  unsigned long now = ((rtc.getMinute() * 60) + rtc.getSecond());
  static unsigned long nextTrigger = 0;
  if (now % TOTP_PERIOD == 0 && now != nextTrigger) {
    generate_totps();
    refresh_totp_labels();
    nextTrigger = now;
  }


  // NOTE: ensures the counter is updated on every second, instead of after 1 second
  static unsigned long previousSecond = 0;
  unsigned long currentSecond = rtc.getSecond();
  if (currentSecond != previousSecond) {
      refresh_counter_bars();
      previousSecond = currentSecond;
  }

  // NOTE: semaphore to allow a single processes to work with the sd card
  if(processMqttMessage){
    processMqttMessage = false;
    if (!isWorkingWithSD) {
      isWorkingWithSD = true;
      add_new_secret(globalPayload.payload, globalPayload.length);
      isWorkingWithSD = false;
    }
  }

  display_and_touch_handler();
}

