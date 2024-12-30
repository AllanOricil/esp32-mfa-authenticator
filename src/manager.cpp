#include "manager.hpp"

httpd_handle_t server = NULL;

esp_err_t serve_file_handler(httpd_req_t *req, const char *path, const char *content_type)
{
	File file = SPIFFS.open(path, FILE_READ);
	if (!file)
	{
		Serial.printf("File not found: %s\n", path);
		httpd_resp_send_404(req);
		return ESP_FAIL;
	}

	httpd_resp_set_type(req, content_type);

	char buffer[512];
	size_t read_bytes;

	while ((read_bytes = file.readBytes(buffer, sizeof(buffer))) > 0)
	{
		if (httpd_resp_send_chunk(req, buffer, read_bytes) != ESP_OK)
		{
			Serial.printf("Failed to send chunk for %s\n", path);
			httpd_resp_send_chunk(req, NULL, 0);
			file.close();
			return ESP_FAIL;
		}
	}

	file.close();
	httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

esp_err_t redirect_handler(httpd_req_t *req)
{
	httpd_resp_set_status(req, "302 Found");
	httpd_resp_set_hdr(req, "Location", "/esp32");
	httpd_resp_send(req, NULL, 0);
	return ESP_OK;
}

esp_err_t esp32_handler(httpd_req_t *req)
{
	return serve_file_handler(req, "/index.html", "text/html");
}

esp_err_t favicon_handler(httpd_req_t *req)
{
	return serve_file_handler(req, "/favicon.ico", "image/x-icon");
}

esp_err_t not_found_handler(httpd_req_t *req)
{
	return serve_file_handler(req, "/404.html", "text/html");
}

esp_err_t get_config_handler(httpd_req_t *req)
{
	Configuration config = Configuration::load();
	String json = config.serializeToJson(true);
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, json.c_str(), json.length());
	return ESP_OK;
}

esp_err_t put_config_handler(httpd_req_t *req)
{
	char buffer[1024];
	int ret = httpd_req_recv(req, buffer, sizeof(buffer));
	if (ret <= 0)
	{
		httpd_resp_send_500(req);
		return ESP_FAIL;
	}

	try
	{
		buffer[ret] = '\0';
		String data_json(buffer);
		Configuration new_config = Configuration::parse(data_json);
		if (new_config.save())
		{
			Serial.printf("Configuration updated successfully.\n");
			httpd_resp_set_type(req, "application/json");
			httpd_resp_send(req, "{\"message\":\"configuration updated\"}", HTTPD_RESP_USE_STRLEN);
			return ESP_OK;
		}
	}
	catch (const std::exception &e)
	{
		Serial.printf("Exception: %s\n", e.what());
		httpd_resp_set_type(req, "application/json");
		httpd_resp_send(req, "{\"message\":\"something went wrong\"}", HTTPD_RESP_USE_STRLEN);
		return ESP_FAIL;
	}

	httpd_resp_send_500(req);
	return ESP_FAIL;
}

void configure_routes()
{
	Serial.println("Configuring server routes");
	httpd_uri_t uri_redirect = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = redirect_handler,
		.user_ctx = NULL};
	httpd_register_uri_handler(server, &uri_redirect);

	httpd_uri_t uri_esp32 = {
		.uri = "/esp32",
		.method = HTTP_GET,
		.handler = esp32_handler,
		.user_ctx = NULL};
	httpd_register_uri_handler(server, &uri_esp32);

	httpd_uri_t uri_favicon = {
		.uri = "/favicon.ico",
		.method = HTTP_GET,
		.handler = favicon_handler,
		.user_ctx = NULL};
	httpd_register_uri_handler(server, &uri_favicon);

	httpd_uri_t uri_config_get = {
		.uri = "/api/v1/config",
		.method = HTTP_GET,
		.handler = get_config_handler,
		.user_ctx = NULL};
	httpd_register_uri_handler(server, &uri_config_get);

	httpd_uri_t uri_config_put = {
		.uri = "/api/v1/config",
		.method = HTTP_PUT,
		.handler = put_config_handler,
		.user_ctx = NULL};
	httpd_register_uri_handler(server, &uri_config_put);

	Serial.println("Server routes configured");
}

void has_null_character(const String &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '\0')
		{
			Serial.printf("HAS NULL TERM CHARACTER\n");
			return;
		}
	}
	Serial.printf("DOES NOT HAVE NULL TERM CHARACTER\n");
	return;
}

void init_manager()
{
	Serial.printf("Initializing manager server.\n");
	if (
		!file_exists(SPIFFS, "/index.html") ||
		!file_exists(SPIFFS, "/200.html") ||
		!file_exists(SPIFFS, "/404.html") ||
		!file_exists(SPIFFS, "/favicon.ico"))
	{
		Serial.printf("Failed to find required files. Server initialization aborted.\n");
		return;
	}

	// NOTE: if there is no cert and key, start HTTP
	if (!file_exists(SD, "/cert.pem") || !file_exists(SD, "/key.pem"))
	{
		Serial.printf("Certificate or key file missing. Starting HTTP server instead.\n");

		httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();
		if (httpd_start(&server, &http_config) != ESP_OK)
		{
			Serial.printf("Failed to start HTTP server.\n");
			return;
		}
	}
	else
	{

		String cert_file_content = read_file(SD, "/cert.pem");
		has_null_character(cert_file_content);
		size_t cert_file_content_size = cert_file_content.length() + 1;
		char *cert_file_content_char = new char[cert_file_content_size];
		cert_file_content.toCharArray(cert_file_content_char, cert_file_content_size);

		String key_file_content = read_file(SD, "/key.pem");
		has_null_character(cert_file_content);
		size_t key_file_content_size = key_file_content.length() + 1;
		char *key_file_content_char = new char[key_file_content_size];
		key_file_content.toCharArray(key_file_content_char, key_file_content_size);

		httpd_ssl_config_t https_config = HTTPD_SSL_CONFIG_DEFAULT();
		https_config.cacert_pem = (uint8_t *)cert_file_content_char;
		https_config.cacert_len = cert_file_content_size;
		https_config.prvtkey_pem = (uint8_t *)key_file_content_char;
		https_config.prvtkey_len = key_file_content_size;

		if (httpd_ssl_start(&server, &https_config) != ESP_OK)
		{
			Serial.printf("Failed to start HTTPS server.\n");
			return;
		}
	}

	configure_routes();

	Serial.printf("HTTPS server started.\n");
}
