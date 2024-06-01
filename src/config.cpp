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

Configuration Configuration::deserialize(const char *filePath)
{
	File file = SD.open(filePath);
	if (!file)
	{
		Serial.println("Error opening config file");
		throw std::runtime_error("Error opening config file");
	}

	Configuration config;
	YAMLNode root = YAMLNode::loadStream(file);
	file.close();

	if (root.isNull())
	{
		Serial.println("Invalid config file format");
		throw "Invalid config file format";
	}

	// WiFi
	if (root["wifi"].isMap())
	{
		config.wifi.ssid = root.gettext("wifi:ssid");
		config.wifi.password = root.gettext("wifi:password");
	}

	// MQTT
	if (root["mqtt"].isMap())
	{
		config.mqtt.server = root.gettext("mqtt:server");
		config.mqtt.port = root.gettext("mqtt:port");
		config.mqtt.username = root.gettext("mqtt:username");
		config.mqtt.password = root.gettext("mqtt:password");
	}

	// Security
	if (root["security"].isMap() && root["security"]["pin"].isMap())
	{
		config.security.pin.hash = root.gettext("security:pin:hash");
		config.security.pin.key = root.gettext("security:pin:key");
	}

	// Touch
	if (root["touch"].isMap())
	{
		const char *forceCalibration = root.gettext("touch:force_calibration");
		config.touch.force_calibration = (forceCalibration && strcmp(forceCalibration, "0") != 0) ? 1 : 0;
	}

	return config;
}

Configuration init_configuration()
{
	Serial.println("Initializing configuration");
	Configuration config = Configuration::deserialize(CONFIG_FILE_PATH);
	Serial.println("Configuration initialized");
	return config;
}
