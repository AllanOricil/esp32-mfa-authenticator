#ifndef BASE32_DECODER_H
#define BASE32_DECODER_H

#include "Base32.h"
#include "totp-map.h"

DecodedBase32Secret decode_encoded_base32_secret(char *secret);
void print_free_memory();

#endif //BASE32_DECODER_H
