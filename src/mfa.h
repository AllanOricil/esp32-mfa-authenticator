#ifndef MFA_H
#define MFA_H

#ifdef __cplusplus
extern "C"
{
#endif
	void update_totps();
#ifdef __cplusplus
}

#include <TOTP.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>
#include <esp_log.h>
#include "services.h"
#include "constants.h"
#include "storage.hpp"
#include "clock.hpp"
#include "utils.hpp"

void add_new_service(volatile uint8_t *payload, unsigned int length);
void load_services();

#endif

#endif // MFA_H