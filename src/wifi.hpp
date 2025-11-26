#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <esp_log.h>
#include "config.hpp"
#include "constants.h"

String init_wifi(const Configuration &config);

#endif // WIFI_H
