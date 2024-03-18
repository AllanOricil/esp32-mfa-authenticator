#include <WiFi.h>
#include <PubSubClient.h>
#include "constants.h"

struct PayloadData {
  byte payload[MQTT_MAX_PAYLOAD_SIZE];
  unsigned int length;
};

WiFiClient espClient;
PubSubClient client(espClient);

char mqttServer[] = MQTT_SERVER;
volatile bool processMqttMessage = false;
volatile PayloadData globalPayload = {{0}, 0};

void print_mqtt_topic_message(char* topic, byte* payload, unsigned int length) {
    // NOTE: Print topic
    Serial.print("MQTT Message arrived on topic: ");
    Serial.println(topic);
    
    // NOTE: Print payload
    Serial.println("With Payload:");
    char message[length+1];
    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.println(message);
}

// NOTE: store received message in memory to process it later
void on_mqtt_message_received(char* topic, byte* payload, unsigned int length) {
  print_mqtt_topic_message(topic, payload, length);
  if(length <= MQTT_MAX_PAYLOAD_SIZE) {
    processMqttMessage = true;
    byte nonVolatilePayload[MQTT_MAX_PAYLOAD_SIZE];
    memcpy(nonVolatilePayload, payload, length);
    for(unsigned int i = 0; i < length; i++) {
        globalPayload.payload[i] = nonVolatilePayload[i];
    }
    globalPayload.length = length;
  }
}

void init_mqtt(){
  client.setServer(mqttServer, MQTT_PORT);
  client.setCallback(on_mqtt_message_received);
}

void connect_to_mqtt(){
	unsigned long currentMillis = millis();
	// NOTE: connect to MQTT topics every 5000 ms
	static unsigned long lastMqttReconnectAttempt = 0;
	if (!client.connected() && (currentMillis - lastMqttReconnectAttempt > MQTT_RECONNECT_INTERVAL)) {
		lastMqttReconnectAttempt = currentMillis;
		Serial.print("Connecting to MQTT...");
		if (client.connect("esp32-mfa-totp-generator")) {
			Serial.println("connected");
			client.subscribe(MQTT_WRITE_NEW_SECRET_TOPIC);
			Serial.println("subscribed to esp32-totp-write-new-secret topic");
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.printf("will try again in %d ms\n", MQTT_RECONNECT_INTERVAL);
		}
	} else {
		client.loop();
	}
}


