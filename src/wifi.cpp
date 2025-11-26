#include "wifi.hpp"

static const char *TAG = "wifi";

String init_wifi(const Configuration &config)
{
  ESP_LOGI(TAG, "initialzing wifi");

  // NOTE: force station mode https://onlinedocs.microchip.com/oxy/GUID-E9773D3D-A940-4268-9E35-189A30B5467E-en-US-1/GUID-857AD752-2DB5-4A73-8EED-D446FB90262C.html
  WiFi.mode(WIFI_STA);
  // NOTE: ensure its state is cleaned on (re)boot
  WiFi.disconnect(true);
  // NOTE: wifi module needs a few ms to let the radio to settle
  delay(200);

  ESP_LOGI(TAG, "connecting to %s", config.wifi.ssid.c_str());
  WiFi.begin(config.wifi.ssid.c_str(), config.wifi.password.c_str());

  // NOTE: stop the initialization after 15s trying to connect to the network
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < WIFI_CONNECTION_TIMEOUT)
  {
    delay(300);
    ESP_LOGI(TAG, ".");
  }

  // NOTE: if it was not possible to connect to the network, return no ip
  if (WiFi.status() != WL_CONNECTED)
  {
    ESP_LOGE(TAG, "wifi connection failed");
    return "";
  }

  String ip = WiFi.localIP().toString();
  ESP_LOGI(TAG, "IP Address: %s", ip.c_str());
  ESP_LOGI(TAG, "wifi initialized");
  return ip;
}
