#include "utils.hpp"

static const char *TAG = "utils";

Secret decode_encoded_base32_secret(const char *encoded_secret)
{
  ESP_LOGD(TAG, "decoding %s", encoded_secret);

  int encoded_secret_length = strlen(encoded_secret);
  uint8_t *encoded_secret_bytes = new uint8_t[encoded_secret_length];
  for (int i = 0; i < encoded_secret_length; i++)
  {
    encoded_secret_bytes[i] = encoded_secret[i];
  }

  uint8_t *decoded_secret_value = NULL;
  Base32 base32;
  int secret_length = base32.fromBase32(encoded_secret_bytes, encoded_secret_length, (uint8_t *&)decoded_secret_value);

  ESP_LOGD(TAG, "decoded base 32 secret");
  for (int i = 0; i < secret_length; i++)
  {
    ESP_LOGD(TAG, "%02X ", decoded_secret_value[i]);
  }
  ESP_LOGD(TAG, "length: %d", secret_length);

  Secret decoded_secret = {
      .length = secret_length,
      .value = decoded_secret_value};

  delete[] encoded_secret_bytes;

  ESP_LOGD(TAG, "secret was decoded successfully");
  return decoded_secret;
}

long last_time_it_outputed_free_memory = 0;
void print_free_memory()
{
  unsigned long current_millis = millis();
  if (current_millis - last_time_it_outputed_free_memory >= 5000)
  {
    ESP_LOGV(TAG, "free memory: %u", xPortGetFreeHeapSize());
    last_time_it_outputed_free_memory = current_millis;
  }
}

int string_2_int(const char *input)
{
  int parsed_input = atoi(input);
  return parsed_input >= 0 ? parsed_input : 0;
}

String format_time_to_UTC_String(time_t timeValue)
{
  char buffer[80];
  struct tm *gmt = gmtime(&timeValue);
  strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
  return String(buffer);
}