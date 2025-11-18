/**************************************************************************
 * Device Switch - Smart Home Framework
 *
 * Fully integrated with the new backend.
 * Non-blocking, supports ON/OFF state via relay or transistor.
 * Auto-publishes state and availability for Home Assistant.
 **************************************************************************/

#ifndef DEVICE_SWITCH_H
#define DEVICE_SWITCH_H

#include <Arduino.h>
#include "framework.h"   // mqtt_subscribe, client

// ==================== MQTT Topics ====================
#define STATE_TOPIC         "home/" DEVICE_ID "/state"
#define COMMAND_TOPIC       "home/" DEVICE_ID "/command"
#define AVAILABILITY_TOPIC  "home/" DEVICE_ID "/availability"
#define DISCOVERY_TOPIC     "homeassistant/switch/" DEVICE_ID "/config"

// ==================== Device Attributes ====================
#define DEVICE_MANUFACTURER "MyBrand"
#define DEVICE_MODEL        "ESP32-Relay"
#define DEVICE_VERSION      "1.0.0"

// ==================== Device State ====================
bool switchState = false;

// ==================== Discovery Payload ====================
const char* discovery_payload = R"json(
{ 
  "name": DEVICE_NAME,
  "uniq_id": DEVICE_ID,
  "state_topic": "home/" DEVICE_ID "/state",
  "command_topic": "home/" DEVICE_ID "/command",
  "availability_topic": "home/" DEVICE_ID "/availability",
  "payload_on": "ON",
  "payload_off": "OFF",
  "state_on": "ON",
  "state_off": "OFF",
  "device": {
    "name": DEVICE_NAME,
    "manufacturer": DEVICE_MANUFACTURER,
    "model": DEVICE_MODEL,
    "sw_version": DEVICE_VERSION
  }
}
)json";

// ==================== Command Handler ====================
void switch_command_handler(String payload) {
    String command = payload;
    command.trim();
    command.toUpperCase();

    if (command == "ON") {
        switchState = true;
        digitalWrite(SWITCH_PIN, HIGH);
        Serial.println("[SWITCH] Turned ON");
    } 
    else if (command == "OFF") {
        switchState = false;
        digitalWrite(SWITCH_PIN, LOW);
        Serial.println("[SWITCH] Turned OFF");
    } 
    else {
        Serial.print("[SWITCH] Unknown command: ");
        Serial.println(command);
        return;
    }

    // Publish updated state
    client.publish(STATE_TOPIC, switchState ? "ON" : "OFF", true);
}

// ==================== Setup ====================
void device_setup() {
    pinMode(SWITCH_PIN, OUTPUT);

    Serial.println("[SWITCH] Device setup...");
    mqtt_subscribe(COMMAND_TOPIC, switch_command_handler);

    // Publish discovery config
    client.publish(DISCOVERY_TOPIC, discovery_payload, true);

    // Publish availability
    client.publish(AVAILABILITY_TOPIC, "online", true);

    // Publish initial state
    client.publish(STATE_TOPIC, switchState ? "ON" : "OFF", true);
}

// ==================== Loop ====================
void device_loop() {
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 10000) {  // every 10 seconds
        lastUpdate = millis();
        client.publish(AVAILABILITY_TOPIC, "online", true);
        client.publish(STATE_TOPIC, switchState ? "ON" : "OFF", true);
    }
}

#endif // DEVICE_SWITCH_H
