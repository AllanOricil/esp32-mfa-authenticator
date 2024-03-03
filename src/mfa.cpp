// Library Includes
#include <TOTP.h>
#include <TimeLib.h>
#include <ESP32Time.h>

// Local Includes
#include "totp-map.h"

extern ESP32Time rtc;
extern char keys[MAX_SIZE][21];
extern DecodedBase32Secret decodedBase32Secrets[MAX_SIZE];

long get_steps() {
  rtc.getTime();
  long t = rtc.getEpoch();
  long steps = t / 30;
  return steps;
}

char* generate_totp(const DecodedBase32Secret decodedBase32Secret) {
  TOTP totp = TOTP((uint8_t*)decodedBase32Secret.value, decodedBase32Secret.length);
  long steps = get_steps();
  char* newTotp = totp.getCodeFromSteps(steps); 
  return newTotp;
}

void generate_totps(){
  for(int i = 0; i < size; i++) {
    char *totp = generate_totp(decodedBase32Secrets[i]);
    upsert_totp(keys[i], totp);
  }
}