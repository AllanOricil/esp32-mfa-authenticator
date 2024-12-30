#ifndef UTILS_H
#define UTILS_H

#include <esp_log.h>
#include <Base32.h>
#include "services.h"

typedef struct
{
	int length;
	uint8_t *value;
} Secret;

Secret decode_encoded_base32_secret(const char *secret);
void print_free_memory();
int string_2_int(const char *input);

#endif // UTILS_H
