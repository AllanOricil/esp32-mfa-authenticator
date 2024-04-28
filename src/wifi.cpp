#include <WiFi.h>

#ifndef WIFI_SSID
  #error "WIFI_SSID is not defined! Please define WIFI_SSID."
#else
  #pragma message "WIFI_SSID is set to: " WIFI_SSID
#endif

#ifndef WIFI_PASSWORD
  #error "WIFI_PASSWORD is not defined! Please define WIFI_PASSWORD."
#else
  #pragma message "WIFI_PASSWORD is set to: " WIFI_PASSWORD
#endif


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
