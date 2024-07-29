#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <FS.h>
#include <SD.h>
#include <YAMLDuino.h>
#include <cstdlib>
#include <ArduinoJson.h>
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
};

#endif