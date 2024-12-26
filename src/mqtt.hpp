#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <stdbool.h>
#include "constants.h"
#include "config.hpp"

struct PayloadData
{
  uint8_t payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

void print_mqtt_topic_message(char *topic, uint8_t *payload, unsigned int length);
void on_mqtt_message_received(char *topic, uint8_t *payload, unsigned int length);
void connect_to_mqtt();
void init_mqtt(Configuration config);

#endif // MQTT_H
