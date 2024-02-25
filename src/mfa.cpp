#include <SPI.h>
#include <SD.h>
#include "Base32.h"
#include <TOTP.h>
#include "totp-map.h"
#include <TimeLib.h>

extern "C" {
  void upsert_totp(char key[], char value[]);
  void print_totps(); 
}

long get_steps() {
  time_t n = now();
  long steps = n / 30;
  Serial.printf("steps: %ld", steps);
  return steps;
}


String generate_totp(const String &secret) {
  Serial.println("generating totp");

  byte* tempDecoded = NULL;
  Base32 base32;

  int secretLength = secret.length();
  byte *secretBytes = new byte[secretLength];
  
  for (int i = 0; i < secretLength; i++) {
    secretBytes[i] = secret[i];
  }
  
  int keyLength = base32.fromBase32(secretBytes, secretLength, (byte*&)tempDecoded);

  Serial.println(keyLength);
  
  TOTP totp = TOTP((uint8_t*)tempDecoded, keyLength);
  
  Serial.println("getting new timestamp");
  long steps = get_steps();
  char* totpCode = totp.getCodeFromSteps(steps); 
  Serial.println(totpCode);

  delete[] tempDecoded; //Don't forget to delete when finished
  delete[] secretBytes;

  String totpString = String(totpCode);
  Serial.println(totpString);
  return totpString;
}


void generate_many_totp(){
  for(int i = 0; i < size; i++) {
    Serial.printf("generating totp for %s\n", keys[i]);
    String totp_string = generate_totp(secrets[i]);
    Serial.printf("totp generated\n");
    char* non_const_cstr = new char[totp_string.length() + 1];
    strcpy(non_const_cstr, totp_string.c_str());
    upsert_totp(keys[i], non_const_cstr);
    Serial.printf("totp map updated\n");
    delete [] non_const_cstr;
  }

  print_totps();
}