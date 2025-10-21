/**
 * @brief WiFi backend for the Smart Home Framework; integration of the Home Assistant API
 *  and Endpoints
 * 
 */

 // Wifi section
#include <WiFi.h>
#include <PubSubClient.h>   // Used for MQTT client
#include "user_input.h"
#include "framework_types.h"

// MQTT client
WifiClient espClient;
PubSubClient client(espClient);

/*
 
   __  __  ___ _____ _____   _____                 _   _                 
  |  \/  |/ _ \_   _|_   _| |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
  | |\/| | | | || |   | |   | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
  | |  | | |_| || |   | |   |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
  |_|  |_|\__\_\|_|   |_|   |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
                                                                         
 
*/ 
/*
subscribe 
callback
reconnect
setup
loop
*/

mqtt_topic_handler_t handlers[MAX_TOPICS];
int handlerCount = 0;

void mqtt_subscribe(const char* topic, mqtt_callback_t callback) {
  if (handlerCount < MAX_TOPICS) {
    handlers[handlerCount++] = {topic, callback};
    client.subscribe(topic);
  }
  else {
    Serial.println("Too many topics defined, change configuration...");
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

  for (int i = 0; i < handlerCount; i++) {
    if (String(topic) == handlers[i].topic) {
      handlers[i].callback(message);
      return;
    }
  }
}

void mqtt_reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Framework")) {
      for (int i = 0; i < handlerCount; i++) client.subscribe(handlers[i].topic);
    } else {
      delay(2000);
    }
  }
}

void framework_setup() {
  Serial.begin(BAUDRATE);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(mqtt_callback);
  user_setup();
}

void framework_loop() {
  if (!client.connected()) mqtt_reconnect();
  client.loop();
  user_loop();
}
