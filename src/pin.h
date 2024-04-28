#ifndef PIN_OPERATIONS_H
#define PIN_OPERATIONS_H

#include <mbedtls/platform.h>
#include <mbedtls/md.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

bool validate_pin(const char *pin);

#endif