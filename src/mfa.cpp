#include "mfa.h"

static const char *TAG = "mfa";

void update_totps()
{
  ESP_LOGI(TAG, "updating totps for the active services group");
  unsigned long elapsed_number_of_time_steps = get_elapsed_number_of_time_steps();

  Service *services = get_services();
  uint8_t active_group = get_active_group();
  for (int i = 0; i < MAX_NUMBER_OF_SERVICES; i++)
  {
    Service *service = &services[i];
    if (service->group == active_group)
    {
      TOTP totp(service->secret.value, service->secret.length);
      char *new_totp = totp.getCodeFromSteps(elapsed_number_of_time_steps);
      update_service_totp_in_active_services_group_by_name(service->name, new_totp);
    }
  }
  ESP_LOGI(TAG, "all tots in the active services group were updated");
}

void load_services()
{
  clear_all_services();
  ESP_LOGI(TAG, "loading services");

  if (!SD.exists(SERVICES_FILE_PATH))
  {
    ESP_LOGE(TAG, "services.yml file does not exist");
    throw std::runtime_error("services.yml file does not exist");
  }

  File file = SD.open(SERVICES_FILE_PATH, FILE_READ);
  if (!file)
  {
    ESP_LOGE(TAG, "error while opening services.yml file");
    throw std::runtime_error("error while opening services.yml file");
  }

  ESP_LOGI(TAG, "reading services.yml");

  YAMLNode root = YAMLNode::loadStream(file);

  if (root.isNull())
  {
    ESP_LOGE(TAG, "services file format is not valid. It must be a valid yml");
    throw std::runtime_error("Services file format is not valid. It must be a valid yml");
  }

  YAMLNode services = root["services"];
  if (services.isNull())
  {
    ESP_LOGE(TAG, "services file does not have any services");
    throw std::runtime_error("services file does not have any services");
  }

  if (services.isSequence())
  {
    for (int i = 0; i < services.size(); ++i)
    {
      ESP_LOGD(TAG, "parsing service %d", i);
      YAMLNode service = services[i];

      if (service["name"].isNull())
      {
        ESP_LOGE(TAG, "service name must not be null");
        throw std::runtime_error("service name must not be null");
      }
      const char *name = service.gettext("name");
      ESP_LOGD(TAG, "name %s", name);

      if (service["secret"].isNull())
      {
        ESP_LOGE(TAG, "service secret must not be null");
        throw std::runtime_error("service secret must not be null");
      }
      const char *secret = service.gettext("secret");
      ESP_LOGD(TAG, "secret %s", secret);
      Secret new_secret = decode_encoded_base32_secret(secret);

      uint8_t service_group = 0;
      if (!service["group"].isNull())
      {
        service_group = (uint8_t)string_2_int(service.gettext("group"));
        if (service_group < 0)
        {
          ESP_LOGD(TAG, "service %s is not configured with group, so its going to be stored in group 0", name);
          service_group = 0;
        }
      }
      ESP_LOGD(TAG, "group %d", service_group);

      upsert_service_by_name_and_group(name, service_group, new_secret.length, new_secret.value);

      ESP_LOGD(TAG, "service was loaded");
    }
  }

  file.close();
  ESP_LOGI(TAG, "all services were loaded");
}

void add_new_service(volatile uint8_t *payload, unsigned int length)
{
  ESP_LOGI(TAG, "adding new service");
  JsonDocument doc;
  char _payload[length + 1];
  memcpy(_payload, const_cast<uint8_t *>(payload), length);
  _payload[length] = '\0';
  auto error = deserializeJson(doc, _payload);
  if (error)
  {
    ESP_LOGE(TAG, "error while deserializing json: %s", error.c_str());
    return;
  }

  if (!doc.containsKey("name") || !doc.containsKey("secret"))
  {
    ESP_LOGE(TAG, "name or secret are missing");
    return;
  }

  const char *service_name = doc["name"];
  ESP_LOGD(TAG, "service name %s", service_name);

  const char *service_encoded32_secret = doc["secret"];
  ESP_LOGD(TAG, "encoded service secret %s", service_encoded32_secret);

  if (!service_name || strlen(service_name) > 60)
  {
    ESP_LOGE(TAG, "service name is empty or bigger than 60 characters");
    return;
  }

  if (!service_encoded32_secret || (strlen(service_encoded32_secret) % 8 != 0))
  {
    ESP_LOGE(TAG, "secret is empty or not multiple of 8");
    return;
  }

  // TODO: write new service to SD. SD Card must be present as the board never stores anything
  load_services();

  ESP_LOGI(TAG, "service added successfully");
}
