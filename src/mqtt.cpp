#include <WiFi.h>
#include <PubSubClient.h>
#include <stdbool.h>
#include "constants.h"

#ifndef MQTT_USERNAME
  #warning "MQTT_USERNAME is not defined! Please define MQTT_USERNAME."
#else
  #pragma message "MQTT_USERNAME is set to: " MQTT_USERNAME
#endif

#ifndef MQTT_PASSWORD
  #warning "MQTT_PASSWORD is not defined! Please define MQTT_PASSWORD."
#else
  #pragma message "MQTT_PASSWORD is set to: " MQTT_PASSWORD
#endif

#ifndef MQTT_SERVER
  #warning "MQTT_SERVER is not defined! Please define MQTT_SERVER."
#elif defined(MQTT_PORT)
  #pragma message "MQTT_SERVER is set to: " MQTT_SERVER
  #pragma message "MQTT_PORT is set to: " MQTT_PORT
  WiFiClient espClient;
  PubSubClient client(espClient);
  #else
  #warning "MQTT_SERVER is defined but MQTT_PORT is not defined! Please define MQTT_PORT."
#endif

struct PayloadData{
  byte payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

bool isMqttConfigured = false;

volatile bool processMqttMessage = false;
volatile PayloadData globalPayload = {{0}, 0};
unsigned long lastReconnectAttempt = 0;


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
  if(strlen(MQTT_USERNAME) && strlen(MQTT_PASSWORD)){
    client.connect(topic, MQTT_USERNAME, MQTT_PASSWORD);
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

void init_mqtt(){
  if(strlen(MQTT_SERVER) && strlen(MQTT_PORT)){
    Serial.println("Configuring MQTT client");
    unsigned long mqtt_port = strtoul(MQTT_PORT, NULL, 10);
    client.setServer(MQTT_SERVER, mqtt_port);
    client.setCallback(on_mqtt_message_received);
    isMqttConfigured = true;
  }
}
