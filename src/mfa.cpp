// Library Includes
#include <TOTP.h>
#include <TimeLib.h>
#include <ESP32Time.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SD.h>

// Local Includes
#include "constants.h"
#include "utils.hpp"
#include "totp-map.h"
#include "file.hpp"

extern ESP32Time rtc;
extern char keys[MAX_NUMBER_OF_SERVICES][MAX_SERVICE_NAME_LENGTH];
extern DecodedBase32Secret decodedBase32Secrets[MAX_NUMBER_OF_SERVICES];
extern int size;

long get_steps() {
  rtc.getTime();
  long t = rtc.getEpoch();
  long steps = t / TOTP_PERIOD;
  Serial.println(rtc.getEpoch());
  return steps;
}

char* generate_totp(const DecodedBase32Secret decodedBase32Secret, long steps) {
  char* result = static_cast<char*>(malloc(MAX_TOTP_LENGTH+1));
  if (result == nullptr) return nullptr;
  TOTP totp = TOTP(static_cast<uint8_t*>(decodedBase32Secret.value), decodedBase32Secret.length);
  snprintf(result, MAX_TOTP_LENGTH+1, totp.getCodeFromSteps(steps));
  return result;
}

void generate_totps() {
  long steps = get_steps();
  for(int i = 0; i < size; i++) {
    char* newTotp = generate_totp(decodedBase32Secrets[i], steps);
    snprintf(totps[i], MAX_TOTP_LENGTH+1, newTotp);
    free(newTotp);
  }
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

void add_new_secret(volatile byte* payload, unsigned int length){
  JsonDocument doc;
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
    load_mfa_totp_keys();
  } else {
    Serial.println("Keys not found in JSON.");
  }
}
