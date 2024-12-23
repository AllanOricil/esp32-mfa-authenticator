#include "wifi.hpp"

void init_wifi(Configuration config)
{
  Serial.printf("Connecting to %s", config.wifi.ssid.c_str());
  WiFi.begin(config.wifi.ssid.c_str(), config.wifi.password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi initialized");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
