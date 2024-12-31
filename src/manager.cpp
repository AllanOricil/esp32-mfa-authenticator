#include "manager.hpp"

static const char *TAG = "manager";

AsyncWebServer server(80);

bool validate_file_exists(const char *path)
{
	if (SPIFFS.exists(path))
	{
		ESP_LOGI(TAG, "file %s exists", path);
		return true;
	}
	else
	{
		ESP_LOGE(TAG, "file %s does not exist", path);
		return false;
	}
}

void init_manager()
{
	ESP_LOGI(TAG, "initializing manager server");

	if (
		!validate_file_exists("/index.html") ||
		!validate_file_exists("/200.html") ||
		!validate_file_exists("/404.html") ||
		!validate_file_exists("/favicon.ico"))
	{
		ESP_LOGE(TAG, "one or more required files are missing. Server initializing aborted");
		return;
	}

	ESP_LOGD(TAG, "configuring routes");

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
			Configuration config = Configuration::load();
			request->send(200, "application/json", config.to_json_string(true));
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
				String data_json = String((char *)data);
				Configuration new_config = Configuration::parse(data_json);
				if (new_config.save())
				{
					ESP_LOGI(TAG, "config updated successfully");
					request->send(200, "application/json", "{\"message\":\"configuration updated\"}");
				}
			}
			catch (const std::exception &e)
			{
				ESP_LOGE(TAG, "error while updating config: %s", e.what());
				request->send(500, "application/json", "{\"message\":\"something went wrong\"}");
			}
		});

	ESP_LOGD(TAG, "server routes configured");

	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

	server.begin();

	ESP_LOGI(TAG, "server initialized");
}