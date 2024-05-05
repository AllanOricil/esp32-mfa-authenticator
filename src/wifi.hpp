#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

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

void init_wifi();

#endif // WIFICONNECTION_H
