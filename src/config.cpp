#include "config.hpp"
#include "constants.h"
#include "utils.hpp"

String Configuration::serializeToJson(bool safe) const
{
	StaticJsonDocument<512> doc;
	doc["version"] = version;

	JsonObject wifiObj = doc.createNestedObject("wifi");
	wifiObj["ssid"] = wifi.ssid;
	wifiObj["password"] = safe ? "*********" : wifi.password;

	JsonObject mqttObj = doc.createNestedObject("mqtt");
	mqttObj["server"] = mqtt.server;
	mqttObj["port"] = string2Int(mqtt.port.c_str());
	mqttObj["username"] = mqtt.username;
	mqttObj["password"] = safe ? "*********" : mqtt.password;

	JsonObject securityObj = doc.createNestedObject("security");
	JsonObject pinObj = securityObj.createNestedObject("pin");
	pinObj["hash"] = safe ? "*********" : security.pin.hash;
	pinObj["key"] = safe ? "*********" : security.pin.key;

	JsonObject displayObj = doc.createNestedObject("display");
	displayObj["sleepTimeout"] = display.sleepTimeout;

	JsonObject touchObj = doc.createNestedObject("touch");
	touchObj["forceCalibration"] = touch.forceCalibration;

	String json;
	serializeJson(doc, json);
	return json;
}

Configuration Configuration::load()
{
	File file = SD.open(CONFIG_FILE_PATH);
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

	if (root["wifi"].isMap())
	{
		if (root["wifi"]["ssid"].isNull())
			throw "Wifi ssid must be defined";
		config.wifi.ssid = root.gettext("wifi:ssid");

		if (root["wifi"]["password"].isNull())
			throw "Wifi password must be defined";
		config.wifi.password = root.gettext("wifi:password");
	}

	if (root["mqtt"].isMap())
	{
		if (!root["mqtt"]["server"].isNull())
			config.mqtt.server = root.gettext("mqtt:server");
		if (!root["mqtt"]["port"].isNull())
		{
			config.mqtt.port = root.gettext("mqtt:port");
		}
		if (!root["mqtt"]["username"].isNull())
			config.mqtt.username = root.gettext("mqtt:username");
		if (!root["mqtt"]["password"].isNull())
			config.mqtt.password = root.gettext("mqtt:password");
	}

	if (root["security"].isMap() && root["security"]["pin"].isMap())
	{
		if (!root["security"]["pin"]["hash"].isNull())
			config.security.pin.hash = root.gettext("security:pin:hash");

		if (!root["security"]["pin"]["key"].isNull())
			config.security.pin.key = root.gettext("security:pin:key");
	}

	if (root["display"].isMap())
	{
		config.display.sleepTimeout = string2Int(root.gettext("display:sleep_timeout"));
	}

	if (root["touch"].isMap())
	{
		const char *forceCalibration = root.gettext("touch:force_calibration");
		config.touch.forceCalibration = forceCalibration && (strcmp(forceCalibration, "true") == 0 || strcmp(forceCalibration, "1") == 0);
	}

	return config;
}

Configuration Configuration::parse(const String &jsonString)
{
	Configuration config;
	StaticJsonDocument<512> jsonDocument;
	DeserializationError error = deserializeJson(jsonDocument, jsonString);
	if (error)
	{
		Serial.print("Failed to parse JSON.");
		Serial.println(error.c_str());
		throw std::runtime_error("Failed to parse JSON.");
	}

	if (jsonDocument.containsKey("version"))
	{
		config.version = jsonDocument["version"].as<String>();
	}

	if (jsonDocument.containsKey("wifi"))
	{
		config.wifi.password = jsonDocument["wifi"]["password"].as<String>();
		config.wifi.ssid = jsonDocument["wifi"]["ssid"].as<String>();
	}

	if (jsonDocument.containsKey("mqtt"))
	{
		config.mqtt.port = jsonDocument["mqtt"]["port"].as<String>();
		config.mqtt.server = jsonDocument["mqtt"]["server"].as<String>();
		config.mqtt.username = jsonDocument["mqtt"]["username"].as<String>();
		config.mqtt.password = jsonDocument["mqtt"]["password"].as<String>();
	}

	if (jsonDocument.containsKey("security"))
	{
		if (jsonDocument["security"].containsKey("pin"))
		{
			config.security.pin.hash = jsonDocument["security"]["pin"]["hash"].as<String>();
			config.security.pin.key = jsonDocument["security"]["pin"]["key"].as<String>();
		}
	}

	if (jsonDocument.containsKey("display"))
	{
		config.display.sleepTimeout = jsonDocument["display"]["sleepTimeout"].as<int>();
	}

	if (jsonDocument.containsKey("touch"))
	{
		config.touch.forceCalibration = jsonDocument["touch"]["forceCalibration"].as<bool>();
	}

	return config;
}

bool Configuration::save() const
{
	Serial.println("saving to file");
	File file = SD.open(CONFIG_FILE_PATH, FILE_WRITE);
	if (!file)
	{
		Serial.println("Error opening config file for writing.");
		return false;
	}

	String configJson = serializeToJson(false);
	StringStream json_input_stream(configJson);
	YAMLNode root = YAMLNode::loadStream(json_input_stream);
	serializeYml(root.getDocument(), file, OUTPUT_YAML);
	serializeYml(root.getDocument(), Serial, OUTPUT_YAML);
	file.close();
	Serial.println("Configuration saved to file successfully.");
	return true;
}
