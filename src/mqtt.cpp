#include <WiFi.h>
#include <PubSubClient.h>
#include <stdbool.h>
#include "constants.h"
#include "config.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

Configuration config;

struct PayloadData{
  byte payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

bool isMqttConfigured = false;
volatile bool processMqttMessage = false;
volatile PayloadData globalPayload = {{0}, 0};
unsigned long lastReconnectAttempt = 0;

bool isMqttServerConfigSet(){
  return !config.mqtt.server.isEmpty() && !config.mqtt.port.isEmpty();
}

bool isMqttCredentialsConfigSet(){
  return !config.mqtt.username.isEmpty() && !config.mqtt.password.isEmpty();
}

void print_mqtt_topic_message(char *topic, byte *payload, unsigned int length){
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
void on_mqtt_message_received(char *topic, byte *payload, unsigned int length){
  print_mqtt_topic_message(topic, payload, length);
  if(length <= MQTT_MAX_PAYLOAD_SIZE){
    processMqttMessage = true;
    byte nonVolatilePayload[MQTT_MAX_PAYLOAD_SIZE];
    memcpy(nonVolatilePayload, payload, length);
    for (unsigned int i = 0; i < length; i++){
      globalPayload.payload[i] = nonVolatilePayload[i];
    }
    globalPayload.length = length;
  }
}

bool connect(const char *topic){
  if(isMqttCredentialsConfigSet()){
    client.connect(topic, config.mqtt.username.c_str(), config.mqtt.password.c_str());
  } else {
    client.connect(topic);
  }

  return client.connected();
}

void connect_to_topic(const char * topic){
  if(connect(topic)){
    Serial.println("Connected");
    client.subscribe(topic);
    Serial.printf("Subscribed to %s\n", topic);
    lastReconnectAttempt = 0;
  } else {
    Serial.printf("Failed, rc=%d. Will try again in %d ms\n", client.state(), MQTT_RECONNECT_INTERVAL);
  }
}

void connect_to_mqtt(){
  if(isMqttConfigured){
    if(!client.connected()){
      unsigned long currentMillis = millis();
      // NOTE: try to connect to MQTT topics every 5000 ms
      if (currentMillis - lastReconnectAttempt > MQTT_RECONNECT_INTERVAL) {
        lastReconnectAttempt = currentMillis;
        connect_to_topic(MQTT_WRITE_NEW_SECRET_TOPIC);
      }
    } else {
      client.loop();
    }
  }
}

void init_mqtt(Configuration _config){
  config = _config;
  if(isMqttServerConfigSet()){
    Serial.println("Initializing MQTT client");
    config = _config;
    client.setServer(config.mqtt.server.c_str(), config.mqtt.port.toInt());
    client.setCallback(on_mqtt_message_received);
    isMqttConfigured = true;
    Serial.println("MQTT client initialized");
  }
}
