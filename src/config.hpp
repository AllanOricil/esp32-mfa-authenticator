#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <YAMLDuino.h>
#include <cstdlib>
#include <ArduinoJson.h>
#include <esp_log.h>
#include "storage.hpp"
#include "utils.hpp"
#include "constants.h"

class Configuration
{
public:
	struct WiFiSettings
	{
		String ssid = "";
		String password = "";
	} wifi;
	struct AuthenticationSettings
	{
		struct PinSettings
		{
			String hash = "";
			String key = "";
		} pin;
		int unlock_attempts = MAX_UNLOCK_ATTEMPTS;
	} authentication;
	struct TouchSettings
	{
		bool calibrate = false;
	} touch;
	struct DisplaySettings
	{
		int sleep_timeout = 10;
	} display;

	String to_json_string(bool safe) const;
	static Configuration load();
	static Configuration parse(const String &jsonString);
	bool save() const;
	bool is_authentication_configured();
};

#endif // CONFIGURATION_H