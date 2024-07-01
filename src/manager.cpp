#include "constants.h"
#include "config.hpp"
#include "manager.hpp"

AsyncWebServer server(80);
Configuration _config;

bool checkFileExists(const char *path)
{
	if (SPIFFS.exists(path))
	{
		Serial.printf("File %s exists.\n", path);
		return true;
	}
	else
	{
		Serial.printf("File %s does not exist!\n", path);
		return false;
	}
}

void init_manager(Configuration config)
{
	Serial.println("Initializing manager server.");
	_config = config;

	if (!SPIFFS.begin(true))
	{
		Serial.println("Something went wrong while mounting SPIFFS.");
		return;
	}

	if (
		!checkFileExists("/index.html") ||
		!checkFileExists("/200.html") ||
		!checkFileExists("/404.html") ||
		!checkFileExists("/favicon.ico"))
	{
		Serial.println("One or more required files are missing. Server initialization aborted.");
		return;
	}

	Serial.println("Configuring routes.");

	// NOTE: assets routes
	server.on(
		"/",
		HTTP_GET,
		[](AsyncWebServerRequest *request)
		{
			request->redirect("/esp32");
		});

	server.on(
		"/esp32",
		HTTP_GET,
		[](AsyncWebServerRequest *request)
		{
			request->send(SPIFFS, "/index.html", "text/html");
		});

	server.onNotFound(
		[](AsyncWebServerRequest *request)
		{
			request->send(SPIFFS, "/404.html", "text/html");
		});

	server.serveStatic("/_nuxt/", SPIFFS, "/_nuxt/");

	server.on(
		"/favicon.ico",
		HTTP_GET,
		[](AsyncWebServerRequest *request)
		{
			request->send(SPIFFS, "/favicon.ico", "image/x-icon");
		});

	// NOTE: api routes
	server.on(
		"/api/v1/config",
		HTTP_GET,
		[](AsyncWebServerRequest *request)
		{
			request->send(200, "application/json", _config.serializeToJson(true));
		});

	server.on(
		"/api/v1/config",
		HTTP_PUT,
		[](AsyncWebServerRequest *request) {},
		nullptr,
		[](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
		{
			try
			{
				String dataJson = String((char *)data);
				Configuration newConfig = Configuration::parse(dataJson);
				if (newConfig.save())
				{
					request->send(200, "application/json", "{\"message\":\"configuration updated\"}");
				}
			}
			catch (const std::exception &e)
			{
				Serial.println(e.what());
				request->send(500, "application/json", "{\"message\":\"something went wrong\"}");
			}
		});

	Serial.println("Routes configured.");

	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

	server.begin();
	Serial.println("Server initialized.");
}