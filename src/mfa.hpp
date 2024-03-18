#pragma once
// Library Includes
#include <TOTP.h>
#include <TimeLib.h>
#include <ESP32Time.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SD.h>

// Local Includes
#include "totp-map.h"
#include "constants.h"
#include "file.hpp"

void generate_totps();
void load_mfa_totp_keys();
void add_new_secret(volatile byte* payload, unsigned int length);