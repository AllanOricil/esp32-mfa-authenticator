#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <YAMLDuino.h>
#include <cstdlib>
#include <ArduinoJson.h>
#include "storage.hpp"
#include "constants.h"
#include "utils.hpp"

class Configuration
{
public:
	String version = "0.0.0";
	struct WiFiSettings
	{
		String ssid = "";
		String password = "";
	} wifi;
	struct MQTTSettings
	{
		String server = "";
		String port = "";
		String username = "";
		String password = "";
	} mqtt;
	struct SecuritySettings
	{
		struct PinSettings
		{
			String hash = "";
			String key = "";
		} pin;
		int maxNumberOfWrongUnlockAttempts = MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS;
	} security;
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
	bool is_mqtt_server_settings_configured();
	bool is_mqtt_topic_credentials_configured();
};

#endif // CONFIGURATION_H