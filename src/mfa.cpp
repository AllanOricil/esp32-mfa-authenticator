#include "mfa.h"

void update_totps()
{
  Serial.println("updating totps for the active services group");
  unsigned long elapsed_number_of_time_steps = get_elapsed_number_of_time_steps();
  for (int i = 0; i < get_active_services_group_length(); i++)
  {
    Service service = get_active_services_group()[i];
    TOTP totp(service.secret.value, service.secret.length);
    char *new_totp = totp.getCodeFromSteps(elapsed_number_of_time_steps);
    upsert_service_totp_in_active_services_group_by_name(service.name, new_totp);
  }
  Serial.println("all tots in the active services group were updated");
}

void load_services()
{
  clear_all_services_groups();
  Serial.println("Loading services");

  if (!SD.exists(SERVICES_FILE_PATH))
  {
    Serial.println("Error the services file does not exist");
    throw std::runtime_error("Error the services file does not exist");
  }

  File file = SD.open(SERVICES_FILE_PATH, FILE_READ);
  if (!file)
  {
    Serial.println("Error opening the services file");
    throw std::runtime_error("Error opening the services file");
  }

  Serial.println("Reading the services file");

  YAMLNode root = YAMLNode::loadStream(file);

  if (root.isNull())
  {
    Serial.println("Services file format is not valid. It must be a valid yml");
    throw "Services file format is not valid. It must be a valid yml";
  }

  YAMLNode services = root["services"];
  if (services.isNull())
  {
    Serial.println("Services file does not have any services");
    throw "Services file does not have any services";
  }

  if (services.isSequence())
  {
    for (int i = 0; i < services.size(); ++i)
    {
      YAMLNode service = services[i];

      if (service["name"].isNull())
      {
        throw std::runtime_error("service name must not be null");
      }
      const char *name = service.gettext("name");

      if (service["secret"].isNull())
      {
        throw std::runtime_error("service secret must not be null");
      }
      const char *secret = service.gettext("secret");
      Secret new_secret = decode_encoded_base32_secret(secret);

      int service_group = 0;
      if (!service["group"].isNull())
      {
        service_group = string_2_int(service.gettext("group"));
        if (service_group < 0 || service_group > (MAX_NUMBER_OF_GROUPS - 1))
        {
          service_group = 0;
        }
      }

      upsert_service_in_group_by_name(service_group, name, new_secret.length, new_secret.value);
    }
  }
  file.close();
  print_all_services_groups();
  Serial.println("all services were loaded");
}

void add_new_service(volatile uint8_t *payload, unsigned int length)
{
  JsonDocument doc;
  char _payload[length + 1];
  memcpy(_payload, const_cast<uint8_t *>(payload), length);
  _payload[length] = '\0';
  auto error = deserializeJson(doc, _payload);
  if (error)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }

  if (!doc.containsKey("name") || !doc.containsKey("secret"))
  {
    Serial.println("Invalid payload.");
    return;
  }

  const char *service_name = doc["name"];
  const char *service_encoded32_secret = doc["secret"];

  Serial.print("Service Name:");
  Serial.println(service_name);

  Serial.print("Secret:");
  Serial.println(service_encoded32_secret);

  if (!service_name || strlen(service_name) > 60)
  {
    Serial.println("Invalid or missing service");
    return;
  }

  if (!service_encoded32_secret || (strlen(service_encoded32_secret) != 16 && strlen(service_encoded32_secret) != 32 && strlen(service_encoded32_secret) != 64))
  {
    Serial.println("Secret is invalid or empty");
    return;
  }

  // TODO: write new service to SD. SD Card must be present as the board never stores anything
  load_services();
}
