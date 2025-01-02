#include "wifi.hpp"

static const char *TAG = "wifi";

String init_wifi(Configuration config)
{
  ESP_LOGI(TAG, "initialzing wifi");
  ESP_LOGI(TAG, "connecting to %s", config.wifi.ssid.c_str());
  WiFi.begin(config.wifi.ssid.c_str(), config.wifi.password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    ESP_LOGI(TAG, ".");
  }
  ESP_LOGI(TAG, "IP Address: %s", WiFi.localIP().toString().c_str());
  ESP_LOGI(TAG, "wifi initialized");
  return WiFi.localIP().toString();
}
