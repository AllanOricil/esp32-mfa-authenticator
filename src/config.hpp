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
	String version = "0.0.0";
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
		int maxNumberOfWrongUnlockAttempts = MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS;
	} authentication;
	struct TouchSettings
	{
		bool forceCalibration = false;
	} touch;
	struct DisplaySettings
	{
		int sleepTimeout = 10;
	} display;

	String serializeToJson(bool safe) const;
	static Configuration load();
	static Configuration parse(const String &jsonString);
	bool save() const;
	bool is_secure();
};

#endif // CONFIGURATION_H