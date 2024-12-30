#ifndef MANAGER_H
#define MANAGER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <esp_log.h>
#include "constants.h"
#include "config.hpp"

void init_manager();

#endif // MANAGER_H