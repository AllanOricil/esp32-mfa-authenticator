#include <FS.h>
#include <SD.h>
#include <YAMLDuino.h>
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
	} security;
	struct TouchSettings
	{
		bool forceCalibration = false;
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
		if (root["wifi"]["ssid"].isNull())
			throw "Wifi ssid must be defined";
		config.wifi.ssid = root.gettext("wifi:ssid");

		if (root["wifi"]["password"].isNull())
			throw "Wifi password must be defined";
		config.wifi.password = root.gettext("wifi:password");
	}

	// MQTT
	if (root["mqtt"].isMap())
	{
		if (!root["mqtt"]["server"].isNull())
			config.mqtt.server = root.gettext("mqtt:server");
		if (!root["mqtt"]["port"].isNull())
			config.mqtt.port = root.gettext("mqtt:port");
		if (!root["mqtt"]["username"].isNull())
			config.mqtt.username = root.gettext("mqtt:username");
		if (!root["mqtt"]["password"].isNull())
			config.mqtt.password = root.gettext("mqtt:password");
	}

	// Security
	if (root["security"].isMap() && root["security"]["pin"].isMap())
	{
		if (!root["security"]["pin"]["hash"].isNull())
			config.security.pin.hash = root.gettext("security:pin:hash");

		if (!root["security"]["pin"]["key"].isNull())
			config.security.pin.key = root.gettext("security:pin:key");
	}

	// Touch
	if (root["touch"].isMap())
	{
		const char *forceCalibration = root.gettext("touch:force_calibration");
		config.touch.forceCalibration = forceCalibration && (strcmp(forceCalibration, "true") == 0 || strcmp(forceCalibration, "1") == 0);
	}

	return config;
}

Configuration init_configuration()
{
	Serial.println("Initializing configuration.");
	Configuration config = Configuration::deserialize(CONFIG_FILE_PATH);
	Serial.println("Configuration initialized.");
	return config;
}
