#include "pin.h"

static char *pin_hash = NULL;
static char *pin_key = NULL;

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
		printf("%02x", hash[i]);
	}
	printf("\n");
}

bool validate_pin(const char *pin)
{
	if (strlen(pin) < PIN_MIN_LENGTH)
		return false;

	unsigned char generated_hash[32];
	mbedtls_md_context_t ctx;
	mbedtls_md_init(&ctx);
	mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
	mbedtls_md_hmac_starts(&ctx, pin_key, strlen((char *)pin_key));
	mbedtls_md_hmac_update(&ctx, (const unsigned char *)pin, strlen(pin));
	mbedtls_md_hmac_finish(&ctx, generated_hash);
	mbedtls_md_free(&ctx);

	print_hash(generated_hash, sizeof(generated_hash) / sizeof(generated_hash[0]));

	return memcmp(generated_hash, hex_to_bin(pin_hash), sizeof(generated_hash)) == 0;
}

void init_pin(const char *hash, const char *key)
{
	pin_hash = strdup(hash);
	pin_key = strdup(key);
}