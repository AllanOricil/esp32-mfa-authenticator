#include "auth.h"

static const char *TAG = "auth";

char *pin_hash = NULL;
char *pin_key = NULL;
char *manager_username = NULL;
char *manager_password = NULL;
char *manager_key = NULL;

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

static bool generate_hmac(const char *key, const char *data, unsigned char *output, size_t output_size)
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

bool validate_pin(const char *pin)
{
	if (strlen(pin) < PIN_MIN_LENGTH)
		return false;

	unsigned char generated_hash[32];
	if (!generate_hmac((char *)pin_key, pin, generated_hash, sizeof(generated_hash)))
	{
		return false;
	}

	return memcmp(generated_hash, hex_to_bin(pin_hash), sizeof(generated_hash)) == 0;
}

bool authenticate(const char *username, const char *password)
{
	if (username == NULL)
	{
		ESP_LOGE(TAG, "username is empty");
		return false;
	}

	if (password == NULL)
	{
		ESP_LOGE(TAG, "password is empty");
		return false;
	}

	if (strcmp(username, manager_username) != 0)
	{
		ESP_LOGW(TAG, "invalid username");
		return false;
	}

	unsigned char generated_hash[32];
	if (!generate_hmac(manager_key, password, generated_hash, sizeof(generated_hash)))
	{
		return false;
	}

	unsigned char *stored_hash = hex_to_bin(manager_password);
	if (stored_hash == NULL)
	{
		ESP_LOGE(TAG, "failed to convert stored password hash to binary");
		return false;
	}

	bool is_valid = memcmp(generated_hash, stored_hash, sizeof(generated_hash)) == 0;

	free(stored_hash);

	if (!is_valid)
	{
		ESP_LOGW(TAG, "invalid password");
	}

	return is_valid;
}

void init_auth(const char *pin_hash, const char *pin_key, const char *manager_username, const char *manager_password, const char *manager_key)
{
	pin_hash = strdup(pin_hash);
	pin_key = strdup(pin_key);
	manager_username = strdup(manager_username);
	manager_password = strdup(manager_password);
	manager_key = strdup(manager_key);
}