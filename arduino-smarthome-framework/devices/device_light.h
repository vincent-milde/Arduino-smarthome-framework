/**************************************************************************
 * Device Light - Smart Home Framework
 *
 * Fully integrated with the new backend.
 * Non-blocking, supports ON/OFF and optional brightness.
 * Auto-publishes state and availability.
 **************************************************************************/

#ifndef DEVICE_LIGHT_H
#define DEVICE_LIGHT_H

#include <Arduino.h>
#include "config.h"      // DEVICE_NAME, DEVICE_ID, DEVICE_ROOM, etc.
#include "framework.h"   // mqtt_subscribe, client

// ==================== MQTT Topics ====================
#define STATE_TOPIC       "home/" DEVICE_ID "/state"
#define COMMAND_TOPIC     "home/" DEVICE_ID "/command"
#define AVAILABILITY_TOPIC "home/" DEVICE_ID "/availability"
#define DISCOVERY_TOPIC   "homeassistant/light/" DEVICE_ID "/config"

// ==================== Device Attributes ====================
#define DEVICE_MANUFACTURER "MyBrand"
#define DEVICE_MODEL        "ESP32-Advanced"
#define DEVICE_VERSION      "1.0.0"


// ==================== Device State ====================
bool lightState = false;
uint8_t brightness = 255;  // 0-255
uint8_t red, green, blue = 255; // For future color support


// ==================== Simple Light ====================
//
//  Basic ON/OFF light without brightness or color support
//
//
#if !defined(BRIGHTNESS_SUPPORT) && !defined(COLOR_SUPPORT)
    #ifdef AUTO_DISCOVERY
        const char* discovery_payload = R"json(
        {
        "name": DEVICE_NAME,
        "uniq_id": DEVICE_ID,
        "state_topic": "home/" DEVICE_ID "/state",
        "command_topic": "home/" DEVICE_ID "/command",
        "availability_topic": "home/" DEVICE_ID "/availability",
        "payload_on": "ON",
        "payload_off": "OFF",
        "device": {
            "name": DEVICE_NAME,
            "manufacturer": DEVICE_MANUFACTURER,
            "model": DEVICE_MODEL,
            "sw_version": DEVICE_VERSION
        }
        }
        )json";
    #endif
    void light_handler(const char* topic, const char* payload) {
        String command = String(payload);
        command.trim();
        command.toUpperCase();

        if (command == "ON") {
            lightState = true;
            digitalWrite(LIGHT_PIN, HIGH);
            Serial.println("[LIGHT] Turned ON");
        } 
        else if (command == "OFF") {
            lightState = false;
            digitalWrite(LIGHT_PIN, LOW);
            Serial.println("[LIGHT] Turned OFF");
        } 
        else {
            Serial.print("[LIGHT] Unknown command: ");
            Serial.println(command);
            return;
        }

        // Publish updated state to MQTT
        client.publish(STATE_TOPIC, lightState ? "ON" : "OFF", true); 
    }


#endif
// ==================== Brightness Light ====================
#if defined(BRIGHTNESS_SUPPORT) && !defined(COLOR_SUPPORT)
#ifdef AUTO_DISCOVERY
    const char* discovery_payload = R"json(
    {
    "name": DEVICE_NAME,
    "uniq_id": DEVICE_ID,
    "state_topic": "home/" DEVICE_ID "/state",
    "command_topic": "home/" DEVICE_ID "/command",
    "availability_topic": "home/" DEVICE_ID "/availability",
    "brightness": true,
    "device": {
        "name": DEVICE_NAME,
        "manufacturer": DEVICE_MANUFACTURER,
        "model": DEVICE_MODEL,
        "sw_version": DEVICE_VERSION
    }
    }
    )json";
#endif
void light_handler(const char* topic, const char* payload) {
  String command = String(payload);
  command.trim();
  command.toUpperCase();

  if (command == "ON") {
        lightState = true;
        analogWrite(LIGHT_PIN, brightness);
    } 
    else if (command == "OFF") {
        lightState = false;
        analogWrite(LIGHT_PIN, 0);
    } 
    else if (command.toInt() >= 0 && command.toInt() <= 255) {
        brightness = command.toInt();
        if (lightState)
        analogWrite(LIGHT_PIN, brightness);
    } 
    else {
        Serial.printf("[LIGHT] Unknown command: %s\n", payload);
        return;
    } 

  String statePayload = "{\"state\":\"" + String(lightState ? "ON" : "OFF") + "\",\"brightness\":" + String(brightness) + "}";
  client.publish(STATE_TOPIC, statePayload.c_str(), true);
}
#endif

// ==================== Color Light ====================
#if defined(COLOR_SUPPORT)
#include <ArduinoJson.h>

const char* discovery_payload = R"json(
{
  "name": DEVICE_NAME,
  "uniq_id": DEVICE_ID,
  "schema": "json",
  "state_topic": "home/" DEVICE_ID "/state",
  "command_topic": "home/" DEVICE_ID "/command",
  "availability_topic": "home/" DEVICE_ID "/availability",
  "brightness": true,
  "rgb": true,
  "device": {
    "name": DEVICE_NAME,
    "manufacturer": DEVICE_MANUFACTURER,
    "model": DEVICE_MODEL,
    "sw_version": DEVICE_VERSION
  }
}
)json";

void light_handler(const char* topic, const char* payload) {
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, payload);

  if (err) {
    Serial.printf("[LIGHT] Invalid JSON: %s\n", err.c_str());
    return;
  }

  if (doc.containsKey("state")) {
    String state = doc["state"];
    state.toUpperCase();
    lightState = (state == "ON");
  }

  if (doc.containsKey("brightness")) {
    brightness = doc["brightness"];
  }

  if (doc.containsKey("color")) {
    red   = doc["color"]["r"];
    green = doc["color"]["g"];
    blue  = doc["color"]["b"];
  }

  if (lightState) {
    // Example: if using PWM channels on ESP32
    analogWrite(RED_PIN,   map(red, 0, 255, 0, brightness));
    analogWrite(GREEN_PIN, map(green, 0, 255, 0, brightness));
    analogWrite(BLUE_PIN,  map(blue, 0, 255, 0, brightness));
  } else {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  }

  String statePayload = "{\"state\":\"" + String(lightState ? "ON" : "OFF") +
                        "\",\"brightness\":" + String(brightness) +
                        ",\"color\":{\"r\":" + String(red) +
                        ",\"g\":" + String(green) +
                        ",\"b\":" + String(blue) + "}}";

  client.publish(STATE_TOPIC, statePayload.c_str(), true);
}
#endif

// ==================== Setup ====================
void device_setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  Serial.println("[LIGHT] Device setup...");

  mqtt_subscribe(COMMAND_TOPIC, light_handler);
  client.publish(DISCOVERY_TOPIC, discovery_payload, true);
  client.publish(AVAILABILITY_TOPIC, "online", true);
}

// ==================== Loop ====================
void device_loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();

#if defined(COLOR_SUPPORT)
    String statePayload = "{\"state\":\"" + String(lightState ? "ON" : "OFF") +
                          "\",\"brightness\":" + String(brightness) +
                          ",\"color\":{\"r\":" + String(red) +
                          ",\"g\":" + String(green) +
                          ",\"b\":" + String(blue) + "}}";
#elif defined(BRIGHTNESS_SUPPORT)
    String statePayload = "{\"state\":\"" + String(lightState ? "ON" : "OFF") +
                          "\",\"brightness\":" + String(brightness) + "}";
#else
    String statePayload = lightState ? "ON" : "OFF";
#endif

    client.publish(STATE_TOPIC, statePayload.c_str(), true);
  }
}

#endif // DEVICE_LIGHT_H
