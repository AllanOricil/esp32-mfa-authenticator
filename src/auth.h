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

	typedef struct
	{
		char *session_id;
		char *username;
		time_t expiration;
	} session;

	void init_auth(
		const char *pin_hash,
		const char *pin_key,
		const char *manager_username,
		const char *manager_password,
		const char *manager_key,
		const int manager_session_length);
	bool validate_pin(const char *pin);
	session *authenticate(const char *username, const char *password);
	bool validate_session(const char *session_id);
	void destroy_session();

#ifdef __cplusplus
}
#endif

#endif // PIN_H