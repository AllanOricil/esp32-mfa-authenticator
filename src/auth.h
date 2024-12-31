#ifndef PIN_H
#define PIN_H

#include <Arduino.h>
#include <mbedtls/platform.h>
#include <mbedtls/md.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <esp_log.h>
#include "constants.h"

#ifdef __cplusplus
extern "C"
{
#endif

	void init_auth(const char *hash, const char *key);
	bool validate_pin(const char *pin);

#ifdef __cplusplus
}
#endif

#endif // PIN_H