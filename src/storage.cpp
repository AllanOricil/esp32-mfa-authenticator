#include "storage.hpp"

static const char *TAG = "storage";

void init_external_storage()
{
  ESP_LOGI(TAG, "initializing external storage");
  if (!SD.begin(TF_CS))
  {
    ESP_LOGE(TAG, "failed to mount sd card reader");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    ESP_LOGE(TAG, "no sd card attached");
    return;
  }

  ESP_LOGD(TAG, "sd card type: ");
  if (cardType == CARD_MMC)
  {
    ESP_LOGD(TAG, "MMC");
  }
  else if (cardType == CARD_SD)
  {
    ESP_LOGD(TAG, "SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    ESP_LOGD(TAG, "SDHC");
  }
  else
  {
    ESP_LOGD(TAG, "unknown");
  }

  ESP_LOGD(TAG, "SD Card Size: %lluMB", SD.cardSize() / (1024 * 1024));
  ESP_LOGD(TAG, "Total space: %lluMB", SD.totalBytes() / (1024 * 1024));
  ESP_LOGD(TAG, "Used space: %lluMB", SD.usedBytes() / (1024 * 1024));

  ESP_LOGI(TAG, "external storage initialized");
}

void init_internal_storage()
{
  ESP_LOGI(TAG, "initializing internal storage");
  if (!SPIFFS.begin(true))
  {
    ESP_LOGE(TAG, "failed to mount SPIFFS");
    return;
  }

  ESP_LOGI(TAG, "internal storage initialized");
}

void init_storage()
{
  init_internal_storage();
  init_external_storage();
}

bool is_file_available(fs::FS &fs, const char *path)
{
  if (fs.exists(path))
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
