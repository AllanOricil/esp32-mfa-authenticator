#include <TOTP.h>
#include "totp-map.h"
#include <TimeLib.h>

long get_steps() {
  time_t t = time(nullptr);
  Serial.printf("time: %ld\n", t);
  long steps = t / 30;
  Serial.printf("steps: %ld\n", steps);
  return steps;
}

char* generate_totp(const DecodedBase32Secret decodedBase32Secret) {
  Serial.println("generating totp");
  TOTP totp = TOTP((uint8_t*)decodedBase32Secret.value, decodedBase32Secret.length);
  long steps = get_steps();
  char* newTotp = totp.getCodeFromSteps(steps); 
  Serial.printf("generated totp: %s\n", newTotp);
  return newTotp;
}

void generate_many_totp(){
  for(int i = 0; i < size; i++) {
    Serial.printf("generating totp for %s\n", keys[i]);
    char *totp = generate_totp(decodedBase32Secrets[i]);
    Serial.printf("totp generated\n");
    upsert_totp(keys[i], totp);
    Serial.printf("totp map updated\n");
  }
  print_totps();
}