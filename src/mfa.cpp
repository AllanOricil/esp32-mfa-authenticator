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