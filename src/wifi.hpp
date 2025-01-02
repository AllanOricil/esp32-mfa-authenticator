#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <esp_log.h>
#include "config.hpp"

String init_wifi(Configuration config);

#endif // WIFI_H
