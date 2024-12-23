#include "config.hpp"

String Configuration::serializeToJson(bool safe) const
{
	StaticJsonDocument<512> doc;
	doc["version"] = version;

	JsonObject wifiObj = doc.createNestedObject("wifi");
	wifiObj["ssid"] = wifi.ssid;
	wifiObj["password"] = safe ? "*********" : wifi.password;

	JsonObject mqttObj = doc.createNestedObject("mqtt");
	mqttObj["server"] = mqtt.server;
	mqttObj["port"] = string_2_int(mqtt.port.c_str());
	mqttObj["username"] = mqtt.username;
	mqttObj["password"] = safe ? "*********" : mqtt.password;

	JsonObject securityObj = doc.createNestedObject("security");
	JsonObject pinObj = securityObj.createNestedObject("pin");
	pinObj["hash"] = safe ? "*********" : security.pin.hash;
	pinObj["key"] = safe ? "*********" : security.pin.key;
	securityObj["max_number_of_wrong_unlock_attempts"] = security.maxNumberOfWrongUnlockAttempts;

	JsonObject displayObj = doc.createNestedObject("display");
	displayObj["sleep_timeout"] = display.sleepTimeout;

	JsonObject touchObj = doc.createNestedObject("touch");
	touchObj["force_calibration"] = touch.forceCalibration;

	String json;
	serializeJson(doc, json);
	return json;
}

Configuration Configuration::load()
{
	Serial.println("Opening config.yml file");
	File file = SD.open(CONFIG_FILE_PATH);
	if (!file)
	{
		Serial.println("Error opening config file");
		throw std::runtime_error("Error opening config file");
	}

	Serial.println("Reading config.yml file");
	Configuration config;
	YAMLNode root = YAMLNode::loadStream(file);
	file.close();

	if (root.isNull())
	{
		Serial.println("Invalid config file format");
		throw std::runtime_error("Invalid config file format");
	}

	if (root["wifi"].isMap())
	{
		if (root["wifi"]["ssid"].isNull())
			throw std::runtime_error("Wifi ssid must be defined");
		config.wifi.ssid = root.gettext("wifi:ssid");

		if (root["wifi"]["password"].isNull())
			throw std::runtime_error("Wifi password must be defined");
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

	if (root["security"].isMap())
	{
		if (!root["security"]["max_number_of_wrong_unlock_attempts"].isNull())
		{
			int maxNumberOfWrongUnlockAttempts = string_2_int(root.gettext("security:max_number_of_wrong_unlock_attempts"));
			config.security.maxNumberOfWrongUnlockAttempts = maxNumberOfWrongUnlockAttempts > 0 ? maxNumberOfWrongUnlockAttempts
																								: MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS;
		}

		if (root["security"]["pin"].isMap())
		{
			if (!root["security"]["pin"]["hash"].isNull())
				config.security.pin.hash = root.gettext("security:pin:hash");

			if (!root["security"]["pin"]["key"].isNull())
				config.security.pin.key = root.gettext("security:pin:key");
		}
	}

	if (root["display"].isMap())
	{
		if (!root["display"]["sleep_timeout"].isNull())
		{
			int sleepTimeout = string_2_int(root.gettext("display:sleep_timeout"));
			config.display.sleepTimeout = sleepTimeout > 0 ? sleepTimeout
														   : SLEEP_TIMEOUT;
		}
	}

	if (root["touch"].isMap())
	{
		const char *forceCalibration = root.gettext("touch:force_calibration");
		config.touch.forceCalibration = forceCalibration && (strcmp(forceCalibration, "true") == 0 || strcmp(forceCalibration, "1") == 0);
	}

	Serial.println("Config.yml file loaded successfully");
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

		if (jsonDocument["security"].containsKey("maxNumberOfWrongUnlockAttempts"))
		{
			int maxNumberOfWrongUnlockAttempts = jsonDocument["security"]["maxNumberOfWrongUnlockAttempts"].as<int>();
			config.security.maxNumberOfWrongUnlockAttempts = maxNumberOfWrongUnlockAttempts > 0 ? maxNumberOfWrongUnlockAttempts
																								: MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS;
		}
		else
		{
			config.security.maxNumberOfWrongUnlockAttempts = MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS;
		}

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

bool Configuration::is_secure()
{
	return !security.pin.hash.isEmpty() && !security.pin.key.isEmpty();
}

bool Configuration::is_mqtt_server_settings_configured()
{
	return !mqtt.server.isEmpty() && !mqtt.port.isEmpty();
}

bool Configuration::is_mqtt_topic_credentials_configured()
{
	return !mqtt.username.isEmpty() && !mqtt.password.isEmpty();
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
	YAML::StringStream json_input_stream(configJson);
	YAMLNode root = YAMLNode::loadStream(json_input_stream);
	serializeYml(root.getDocument(), file, OUTPUT_YAML);
	serializeYml(root.getDocument(), Serial, OUTPUT_YAML);
	file.close();
	Serial.printf("\nConfiguration saved to file successfully.\n");
	return true;
}
