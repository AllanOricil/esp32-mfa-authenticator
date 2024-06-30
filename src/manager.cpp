#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "constants.h"

AsyncWebServer server(80);

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

bool checkFolderExists(const char *path)
{
	File root = SPIFFS.open("/");
	File file = root.openNextFile();

	while (file)
	{
		if (file.isDirectory())
		{
			if (strcmp(file.name(), path) == 0)
			{
				file.close();
				return true;
			}
		}
		file = root.openNextFile();
	}

	return false;
}

void init_manager()
{
	Serial.println("Initializing manager server.");

	if (!SPIFFS.begin(true))
	{
		Serial.println("something went wrong while mounting SPIFFS");
		return;
	}

	if (
		!checkFileExists("/index.html") ||
		!checkFileExists("/200.html") ||
		!checkFileExists("/404.html") ||
		!checkFileExists("/favicon.ico") ||
		!checkFolderExists("/_nuxt"))
	{
		Serial.println("One or more required files are missing. Server initialization aborted.");
		return;
	}

	Serial.println("Configuring routes.");

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

	Serial.println("Routes configured.");

	server.begin();
	Serial.println("Server initialized.");
}