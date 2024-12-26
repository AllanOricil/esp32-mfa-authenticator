#include "mqtt.hpp"

WiFiClient wifi_client;
PubSubClient client(wifi_client);
Configuration config;
bool is_mqtt_client_configured = false;
volatile bool process_mqtt_message = false;
volatile PayloadData global_payload = {{0}, 0};
unsigned long last_reconnect_attempt = 0;

void print_mqtt_topic_message(char *topic, uint8_t *payload, unsigned int length)
{
  // NOTE: Print topic
  Serial.print("MQTT Message arrived on topic: ");
  Serial.println(topic);

  // NOTE: Print payload
  Serial.println("With Payload:");
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.println(message);
}

// NOTE: store received message in memory to process it later
void on_mqtt_message_received(char *topic, uint8_t *payload, unsigned int length)
{
  print_mqtt_topic_message(topic, payload, length);
  if (length <= MQTT_MAX_PAYLOAD_SIZE)
  {
    process_mqtt_message = true;
    uint8_t payload_copy[MQTT_MAX_PAYLOAD_SIZE];
    memcpy(payload_copy, payload, length);
    for (unsigned int i = 0; i < length; i++)
    {
      global_payload.payload[i] = payload_copy[i];
    }
    global_payload.length = length;
  }
}

bool connect(const char *topic)
{
  if (config.is_mqtt_topic_credentials_configured())
  {
    client.connect(topic, config.mqtt.username.c_str(), config.mqtt.password.c_str());
  }
  else
  {
    client.connect(topic);
  }

  return client.connected();
}

void connect_to_topic(const char *topic)
{
  if (connect(topic))
  {
    Serial.println("Connected");
    client.subscribe(topic);
    Serial.printf("Subscribed to %s\n", topic);
    last_reconnect_attempt = 0;
  }
  else
  {
    Serial.printf("Failed, rc=%d. Will try again in %d ms\n", client.state(), MQTT_RECONNECT_INTERVAL);
  }
}

void connect_to_mqtt()
{
  if (config.is_mqtt_server_settings_configured())
  {
    if (!client.connected())
    {
      unsigned long current_time = millis();
      // NOTE: try to connect to MQTT topics every 5000 ms
      if (current_time - last_reconnect_attempt > MQTT_RECONNECT_INTERVAL)
      {
        last_reconnect_attempt = current_time;
        connect_to_topic(MQTT_WRITE_NEW_SECRET_TOPIC);
      }
    }
    else
    {
      client.loop();
    }
  }
}

void init_mqtt(Configuration _config)
{
  config = _config;
  if (config.is_mqtt_server_settings_configured())
  {
    Serial.println("Initializing MQTT client");
    client.setServer(config.mqtt.server.c_str(), config.mqtt.port.toInt());
    client.setCallback(on_mqtt_message_received);
    is_mqtt_client_configured = true;
    Serial.println("MQTT client initialized");
  }
}
