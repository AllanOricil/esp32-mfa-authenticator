#ifndef MANAGER_H
#define MANAGER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <esp_log.h>
#include "auth.h"
#include "constants.h"
#include "config.hpp"
#include "utils.hpp"
#include "storage.hpp"

void init_manager(Configuration config, const char *local_network_ip);

#endif // MANAGER_H