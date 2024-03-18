#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

#ifndef WIFI_SSID
#define WIFI_SSID (char*)"CHOCOLATE"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD (char*)"CHOCOLATE"
#endif

void init_wifi();

#endif //WIFICONNECTION_H 
