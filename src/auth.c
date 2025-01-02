#include "auth.h"

static const char *TAG = "auth";

char *_pin_hash = NULL;
char *_pin_key = NULL;
char *_manager_username = NULL;
char *_manager_password = NULL;
char *_manager_key = NULL;
int _manager_session_length = MAX_MANAGER_SESSION_LENGTH;

session *current_session = NULL;

static char *generate_session_id()
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t length = 32;
	char *session_id = malloc(length + 1);

	if (session_id == NULL)
		return NULL;

	for (size_t i = 0; i < length; i++)
	{
		session_id[i] = charset[rand() % (sizeof(charset) - 1)];
	}
	session_id[length] = '\0';

	return session_id;
}

bool create_session(const char *username)
{
	if (current_session != NULL)
	{
		ESP_LOGW(TAG, "session already exists. Destroying existing session.");
		destroy_session();
	}

	current_session = malloc(sizeof(session));
	if (current_session == NULL)
	{
		ESP_LOGE(TAG, "failed to allocate memory for session.");
		return false;
	}

	current_session->session_id = generate_session_id();
	current_session->username = strdup(username);
	current_session->expiration = time(NULL) + _manager_session_length * 60;

	if (current_session->session_id == NULL || current_session->username == NULL)
	{
		ESP_LOGE(TAG, "failed to create session.");
		destroy_session();
		return false;
	}

	ESP_LOGI(TAG, "session created for user: %s", username);
	ESP_LOGI(TAG, "session ID: %s", current_session->session_id);

	return true;
}

bool validate_session(const char *session_id)
{
	if (current_session == NULL)
	{
		ESP_LOGW(TAG, "mo active session.");
		return false;
	}

	if (strcmp(current_session->session_id, session_id) != 0)
	{
		ESP_LOGW(TAG, "invalid session ID.");
		return false;
	}

	if (time(NULL) > current_session->expiration)
	{
		ESP_LOGW(TAG, "session expired.");
		destroy_session();
		return false;
	}

	ESP_LOGI(TAG, "session is valid for user: %s", current_session->username);
	return true;
}

void destroy_session()
{
	if (current_session != NULL)
	{
		free(current_session->session_id);
		free(current_session->username);
		free(current_session);
		current_session = NULL;

		ESP_LOGI(TAG, "session destroyed.");
	}
}

unsigned char *hex_to_bin(const char *hex_string)
{
	size_t len = strlen(hex_string);
	if (len % 2 != 0)
	{
		return NULL;
	}

	size_t bin_len = len / 2;
	unsigned char *bin_data = (unsigned char *)malloc(bin_len);
	if (bin_data == NULL)
	{
		return NULL;
	}

	for (size_t i = 0; i < len; i += 2)
	{
		char byte[3] = {hex_string[i], hex_string[i + 1], '\0'};
		bin_data[i / 2] = strtol(byte, NULL, 16);
	}

	return bin_data;
}

void print_hash(unsigned char *hash, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		ESP_LOGD(TAG, "%02x", hash[i]);
	}
}

static bool hash(const char *key, const char *data, unsigned char *output, size_t output_size)
{
	if (key == NULL || data == NULL || output == NULL || output_size < 32)
	{
		ESP_LOGE(TAG, "invalid parameters for HMAC generation");
		return false;
	}

	mbedtls_md_context_t ctx;
	mbedtls_md_init(&ctx);

	if (mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1) != 0)
	{
		ESP_LOGE(TAG, "failed to set up HMAC");
		mbedtls_md_free(&ctx);
		return false;
	}

	if (mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, strlen(key)) != 0)
	{
		ESP_LOGE(TAG, "failed to start HMAC");
		mbedtls_md_free(&ctx);
		return false;
	}

	if (mbedtls_md_hmac_update(&ctx, (const unsigned char *)data, strlen(data)) != 0)
	{
		ESP_LOGE(TAG, "failed to update HMAC");
		mbedtls_md_free(&ctx);
		return false;
	}

	if (mbedtls_md_hmac_finish(&ctx, output) != 0)
	{
		ESP_LOGE(TAG, "failed to finish HMAC");
		mbedtls_md_free(&ctx);
		return false;
	}

	mbedtls_md_free(&ctx);

	ESP_LOGD(TAG, "generated password hash:");
	print_hash(output, output_size);

	return true;
}

session *authenticate(const char *username, const char *password)
{
	if (username == NULL)
	{
		ESP_LOGE(TAG, "username is empty");
		return NULL;
	}

	if (password == NULL)
	{
		ESP_LOGE(TAG, "password is empty");
		return NULL;
	}

	if (strcmp(username, _manager_username) != 0)
	{
		ESP_LOGW(TAG, "invalid username");
		return NULL;
	}

	unsigned char generated_hash[32];
	if (!hash(_manager_key, password, generated_hash, sizeof(generated_hash)))
	{
		ESP_LOGE(TAG, "failed to hash password");
		return NULL;
	}

	if (memcmp(generated_hash, hex_to_bin(_manager_password), sizeof(generated_hash)) != 0)
	{
		ESP_LOGE(TAG, "invalid password");
		return NULL;
	}

	if (!create_session(username))
	{
		return NULL;
	}

	return current_session;
}

bool validate_pin(const char *pin)
{
	if (strlen(pin) < PIN_MIN_LENGTH)
		return false;

	unsigned char generated_hash[32];
	if (!hash(_pin_key, pin, generated_hash, sizeof(generated_hash)))
	{
		return false;
	}

	return memcmp(generated_hash, hex_to_bin(_pin_hash), sizeof(generated_hash)) == 0;
}

void init_auth(
	const char *pin_hash,
	const char *pin_key,
	const char *manager_username,
	const char *manager_password,
	const char *manager_key,
	const int manager_session_length)
{
	ESP_LOGI(TAG, "initializing auth");
	_pin_hash = strdup(pin_hash);
	_pin_key = strdup(pin_key);
	_manager_username = strdup(manager_username);
	_manager_password = strdup(manager_password);
	_manager_key = strdup(manager_key);
	_manager_session_length = manager_session_length;
	ESP_LOGI(TAG, "auth initialized");
}