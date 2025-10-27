/**
 * @brief WiFi backend for the Smart Home Framework; integration of the Home Assistant API
 *  and Endpoints
 * 
 */

// Wifi section

#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include "user_settings.h" // for MAX_TOPICS and user hooks
#include "framework_types.h"


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

// Global variable definitions
WiFiClient espClient;
PubSubClient client(espClient);
mqtt_topic_handler_t handlers[MAX_TOPICS];
int handlerCount = 0;

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
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Framework-" + String(WiFi.macAddress());
    
    if (client.connect(
          clientId.c_str(),
          MQTT_USER, MQTT_PASSWORD,
          "homeassistant/status", 0, true, "offline"  // LWT (last will)
        )) {
      Serial.println("connected");
      client.publish("homeassistant/status", "online", true);

      // Re-subscribe to topics
      for (int i = 0; i < handlerCount; i++) {
        client.subscribe(handlers[i].topic);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}


void framework_setup() {
  Serial.begin(BAUDRATE);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(mqtt_callback);
}

void framework_loop() {
  if (!client.connected()) mqtt_reconnect();
  client.loop();
}