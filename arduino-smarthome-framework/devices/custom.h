/**************************************************************************
 * Custom Device Configuration
 * 
 * This file is included automatically when DEVICE_TYPE = "custom"
 * 
 * Use this to define your own MQTT topics, payloads, and discovery config.
 * 
 * NOTE: This mode disables automatic discovery payload generation.
 * 
 **************************************************************************/

#ifndef DEVICE_CUSTOM_H
#define DEVICE_CUSTOM_H

// ==================== Custom Topics ====================
//@TODO: Change this vibe coded garbage to something more useful
#define CUSTOM_STATE_TOPIC   "home/custom/device01/state"
#define CUSTOM_COMMAND_TOPIC "home/custom/device01/command"
#define CUSTOM_DISCOVERY_TOPIC "homeassistant/custom/device01/config"

// ==================== Optional Attributes ====================
#define CUSTOM_MANUFACTURER  "MyBrand"
#define CUSTOM_MODEL         "ESP32-Advanced"
#define CUSTOM_VERSION       "1.0.0"

// ==================== Custom Discovery JSON ====================
const char* discovery_payload = R"json(
{
  "name": "My Custom Device",
  "uniq_id": "custom_device01",
  "state_topic": "home/custom/device01/state",
  "command_topic": "home/custom/device01/command",
  "device": {
    "name": "My Custom Device",
    "manufacturer": "MyBrand",
    "model": "ESP32-Advanced"
  }
}
)json";

// ==================== Custom Functions ====================

// Called once during setup()
void device_setup() {
  Serial.println("[CUSTOM] Setup called.");
  client.publish(CUSTOM_DISCOVERY_TOPIC, discovery_payload, true);
  client.publish("home/custom/device01/availability", "online", true);
}

// Called in loop()
void custom_loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();
    client.publish(CUSTOM_STATE_TOPIC, "{\"status\":\"ok\"}", true); //example topic
  }
}

#endif // DEVICE_CUSTOM_H
