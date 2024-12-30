#ifndef MANAGER_H
#define MANAGER_H

#include <Arduino.h>
#include <esp_log.h>
#include <esp_https_server.h>
#include "file.hpp"
#include "constants.h"
#include "config.hpp"

#define MBEDTLS_DEBUG_C

void init_manager();

#endif // MANAGER_H