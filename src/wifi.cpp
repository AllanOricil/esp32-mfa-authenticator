#include <WiFi.h>
#include "config.hpp"

void init_wifi(Configuration config)
{
  Serial.printf("Connecting to %s", config.wifi.ssid);
  WiFi.begin(config.wifi.ssid, config.wifi.password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
