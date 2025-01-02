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

String getCookie(AsyncWebServerRequest *request, const String &cookieName)
{
	String cookies = request->header("Cookie");
	int startPos = cookies.indexOf(cookieName + "=");
	if (startPos != -1)
	{
		startPos += cookieName.length() + 1;
		int endPos = cookies.indexOf(";", startPos);
		if (endPos == -1)
		{
			endPos = cookies.length();
		}
		return cookies.substring(startPos, endPos);
	}
	return "";
}

void init_manager(Configuration config, const char *local_network_ip)
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
		"/api/v1/auth/login",
		HTTP_POST,
		[](AsyncWebServerRequest *request) {},
		nullptr,
		[](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
		{
			try
			{
				String data_json = String((char *)data);
				StaticJsonDocument<256> doc;
				DeserializationError error = deserializeJson(doc, data_json);
				if (error)
				{
					ESP_LOGE(TAG, "JSON parsing failed: %s", error.c_str());
					request->send(400, "application/json", "{\"message\":\"invalid JSON\"}");
					return;
				}

				const char *username = doc["username"];
				const char *password = doc["password"];

				if (!username || !password)
				{
					ESP_LOGE(TAG, "missing username or password in JSON");
					request->send(400, "application/json", "{\"message\":\"username or password missing\"}");
					return;
				}

				session *session = authenticate(username, password);
				if (session != nullptr)
				{
					ESP_LOGI(TAG, "authentication successful, session token set");

					// NOTE: only this server can use this session token from js
					String cookie = "esp32_mfa_authenticator_session_id=" + String(session->session_id) + "; Expires=" + format_time_to_UTC_String(session->expiration) + "; Path=/; HttpOnly; SameSite=Strict";
					AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"message\":\"authentication successful\"}");
					response->addHeader("Set-Cookie", cookie);
					request->send(response);
				}
				else
				{
					request->send(401, "application/json", "{\"message\":\"invalid credentials\"}");
				}
			}
			catch (const std::exception &e)
			{
				ESP_LOGE(TAG, "error while authenticating: %s", e.what());
				request->send(500, "application/json", "{\"message\":\"something went wrong\"}");
			}
		});

	server.on(
		"/api/v1/auth/logout",
		HTTP_POST,
		[](AsyncWebServerRequest *request)
		{
			try
			{
				destroy_session();
				String cookie = "esp32_mfa_authenticator_session_id=; Expires=0; Max-Age=0; Path=/; HttpOnly; SameSite=Strict";
				AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"message\":\"logout successful\"}");
				response->addHeader("Set-Cookie", cookie);
				request->send(response);
				ESP_LOGI(TAG, "logout successful, session token cleared");
			}
			catch (const std::exception &e)
			{
				ESP_LOGE(TAG, "error while logging out: %s", e.what());
				request->send(500, "application/json", "{\"message\":\"something went wrong\"}");
			}
		});

	server.on(
		"/api/v1/auth/validate",
		HTTP_POST,
		[](AsyncWebServerRequest *request)
		{
			try
			{
				String session_id = getCookie(request, "esp32_mfa_authenticator_session_id");
				if (session_id != "")
				{
					if (validate_session(session_id.c_str()))
					{
						request->send(200, "application/json", "{\"message\":\"session valid\"}");
						ESP_LOGI(TAG, "session validated successfully");
					}
					else
					{
						request->send(401, "application/json", "{\"message\":\"session invalid\"}");
						ESP_LOGI(TAG, "session invalid");
					}
				}
				else
				{
					request->send(401, "application/json", "{\"message\":\"session not found\"}");
					ESP_LOGI(TAG, "session not found");
				}
			}
			catch (const std::exception &e)
			{
				ESP_LOGE(TAG, "error while validating session: %s", e.what());
				request->send(500, "application/json", "{\"message\":\"something went wrong\"}");
			}
		});

	server.on(
		"/api/v1/config",
		HTTP_GET,
		[config](AsyncWebServerRequest *request)
		{
			request->send(200, "application/json", config.to_json_string(true));
		});

	server.on(
		"/api/v1/config",
		HTTP_PUT,
		[](AsyncWebServerRequest *request) {},
		nullptr,
		[config](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
		{
			try
			{
				String data_json = String((char *)data);
				Configuration new_config = Configuration::parse(data_json);
				new_config.manager = config.manager;
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

	// NOTE: these are some of helmet headers I got from one of many of my servers built with express
	DefaultHeaders::Instance().addHeader("X-Content-Type-Options", "nosniff");
	DefaultHeaders::Instance().addHeader("X-Frame-Options", "DENY");
	DefaultHeaders::Instance().addHeader("X-XSS-Protection", "1; mode=block");
	// TODO: add this if config.manager.baseURL is set with a reverse proxy one ? have to try
	// DefaultHeaders::Instance().addHeader("Strict-Transport-Security", "max-age=31536000; includeSubDomains; preload");
	DefaultHeaders::Instance().addHeader("Referrer-Policy", "no-referrer");
	DefaultHeaders::Instance().addHeader("Permissions-Policy", "geolocation=(), microphone=()");

	// TODO: allow configuring this using config.manager.baseURL
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", String("http://") + local_network_ip);
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Cookie");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
	DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "3600");

	server.begin();

	ESP_LOGI(TAG, "server initialized");
}