#ifndef UTILS_H
#define UTILS_H

#include <esp_log.h>
#include <Base32.h>
#include <time.h>
#include "services.h"

typedef struct
{
	int length;
	uint8_t *value;
} Secret;

Secret decode_encoded_base32_secret(const char *secret);
void print_free_memory();
int string_2_int(const char *input);
String format_time_to_UTC_String(time_t timeValue);

#endif // UTILS_H
