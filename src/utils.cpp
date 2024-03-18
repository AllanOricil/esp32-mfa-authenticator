#include <Base32.h>
#include "totp-map.h"

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

long lastPrint = 0;
void print_free_memory(){
  unsigned long currentMillis = millis();
  if(currentMillis - lastPrint >= 5000) {
    lastPrint = currentMillis;
    Serial.print("Free Memory: ");
    Serial.println(xPortGetFreeHeapSize());
  }
}