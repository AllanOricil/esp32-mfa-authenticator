#ifndef MQTT_INTERFACE_H
#define MQTT_INTERFACE_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "constants.h"

struct PayloadData {
  byte payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

extern WiFiClient espClient;
extern PubSubClient client;

extern char mqttServer[];
extern volatile bool processMqttMessage;
extern volatile PayloadData globalPayload;

void print_mqtt_topic_message(char* topic, byte* payload, unsigned int length);
void on_mqtt_message_received(char* topic, byte* payload, unsigned int length);
void connect_to_mqtt();
void init_mqtt();

#endif // MQTT_INTERFACE_H
