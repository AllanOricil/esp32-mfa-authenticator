#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <FS.h>
#include <SD.h>
#include <YAMLDuino.h>
#include "constants.h"

class Configuration
{
public:
	String version;
	struct WiFiSettings
	{
		String ssid;
		String password;
	} wifi;
	struct MQTTSettings
	{
		String server;
		String port;
		String username;
		String password;
	} mqtt;
	struct SecuritySettings
	{
		struct PinSettings
		{
			String hash;
			String key;
		} pin;
	} security;
	struct TouchSettings
	{
		int force_calibration;
	} touch;

	static Configuration deserialize(const char *filePath);
};

Configuration init_configuration();

#endif