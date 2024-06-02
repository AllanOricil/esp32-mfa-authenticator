#pragma once

#include <mbedtls/platform.h>
#include <mbedtls/md.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

#ifdef __cplusplus
extern "C"
{
#endif

	void init_pin(const char *hash, const char *key);
	bool validate_pin(const char *pin);

#ifdef __cplusplus
}
#endif