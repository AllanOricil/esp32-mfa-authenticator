#include <mbedtls/platform.h>
#include <mbedtls/md.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

bool validate_pin(const char *pin)
{
	if (strlen(pin) < PIN_MIN_LENGTH)
		return false;

	unsigned char generated_hash[32];
	mbedtls_md_context_t ctx;
	mbedtls_md_init(&ctx);
	mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
	mbedtls_md_starts(&ctx);
	mbedtls_md_update(&ctx, (const unsigned char *)pin, strlen(pin));
	mbedtls_md_update(&ctx, PIN_SALT, strlen((char *)PIN_SALT));
	mbedtls_md_finish(&ctx, generated_hash);
	mbedtls_md_free(&ctx);

	return memcmp(generated_hash, PIN_HASH, sizeof(generated_hash)) == 0;
}