// Library Includes
#include <TOTP.h>
#include <TimeLib.h>
#include <ESP32Time.h>

// Local Includes
#include "totp-map.h"
#include "constants.h"

extern ESP32Time rtc;
extern char keys[MAX_NUMBER_OF_SERVICES][MAX_SERVICE_NAME_LENGTH];
extern DecodedBase32Secret decodedBase32Secrets[MAX_NUMBER_OF_SERVICES];

long get_steps() {
  rtc.getTime();
  long t = rtc.getEpoch();
  long steps = t / TOTP_PERIOD;
  Serial.println(rtc.getEpoch());
  return steps;
}

char* generate_totp(const DecodedBase32Secret decodedBase32Secret, long steps) {
  TOTP totp = TOTP((uint8_t*)decodedBase32Secret.value, decodedBase32Secret.length);
  char* newTotp = totp.getCodeFromSteps(steps); 
  return newTotp;
}

void generate_totps(){
  long steps = get_steps();
  for(int i = 0; i < size; i++) {
    char *totp = generate_totp(decodedBase32Secrets[i], steps);
    upsert_totp(keys[i], totp);
  }
}