/**************************************************************************
 * Device Sensor - Smart Home Framework
 *
 * Fully integrated with the new backend.
 * Non-blocking, supports analog and digital sensors with configurable
 * reading intervals. Auto-publishes sensor values and availability.
 * Compatible with Home Assistant MQTT Discovery.
 **************************************************************************/

#ifndef DEVICE_SENSOR_H
#define DEVICE_SENSOR_H

#include <Arduino.h>
#include "framework.h"   // mqtt_subscribe, client

// ==================== Configuration ====================
// Define your sensor configuration here
// Uncomment and configure based on your sensor type

//#define SENSOR_TYPE_TEMPERATURE     // Temperature sensor (e.g., DHT11, DHT22)
//#define SENSOR_TYPE_HUMIDITY        // Humidity sensor (e.g., DHT11, DHT22)
//#define SENSOR_TYPE_ANALOG          // Generic analog input (0-1023)
//#define SENSOR_TYPE_DIGITAL         // Digital input (HIGH/LOW)

#define SENSOR_PIN 34               // Pin connected to the sensor
#define SENSOR_UPDATE_INTERVAL 10000 // Update interval in milliseconds (10 seconds)
#define SENSOR_NAME "Sensor"         // Display name for Home Assistant

// ==================== Device Attributes ====================
#define DEVICE_MANUFACTURER "MyBrand"
#define DEVICE_MODEL        "ESP32-Sensor"
#define DEVICE_VERSION      "1.0.0"

// ==================== Device State ====================
float sensorValue = 0.0;
unsigned long lastSensorRead = 0;

// ==================== Sensor Reading Functions ====================

// Read analog sensor value (0-1023 mapped to 0-100%)
float read_analog_sensor() {
    int rawValue = analogRead(SENSOR_PIN);
    // Map raw value to percentage or custom range
    return (rawValue / 1023.0) * 100.0;  // Returns 0-100%
}

// Read digital sensor value (HIGH/LOW)
bool read_digital_sensor() {
    return digitalRead(SENSOR_PIN) == HIGH;
}

// Example function for temperature/humidity sensors (requires library)
// Uncomment and adapt based on your specific sensor library
/*
float read_temperature_sensor() {
    // Example using DHT sensor
    // float temp = dht.readTemperature();
    // if (isnan(temp)) {
    //     Serial.println("[SENSOR] Failed to read temperature!");
    //     return -999.0;
    // }
    // return temp;
}

float read_humidity_sensor() {
    // Example using DHT sensor
    // float humidity = dht.readHumidity();
    // if (isnan(humidity)) {
    //     Serial.println("[SENSOR] Failed to read humidity!");
    //     return -999.0;
    // }
    // return humidity;
}
*/

// ==================== Main Sensor Reading Logic ====================
void read_sensor() {
    unsigned long currentTime = millis();
    
    // Only read at specified intervals
    if (currentTime - lastSensorRead < SENSOR_UPDATE_INTERVAL) {
        return;
    }
    
    lastSensorRead = currentTime;

    // Read sensor based on type
    #if defined(SENSOR_TYPE_ANALOG)
        sensorValue = read_analog_sensor();
        Serial.printf("[SENSOR] Analog reading: %.2f%%\n", sensorValue);
    
    #elif defined(SENSOR_TYPE_DIGITAL)
        sensorValue = read_digital_sensor() ? 1.0 : 0.0;
        Serial.printf("[SENSOR] Digital reading: %s\n", (sensorValue == 1.0) ? "HIGH" : "LOW");
    
    #elif defined(SENSOR_TYPE_TEMPERATURE)
        // Uncomment and adapt based on your sensor library
        // sensorValue = read_temperature_sensor();
        // Serial.printf("[SENSOR] Temperature: %.2f°C\n", sensorValue);
        Serial.println("[SENSOR] Temperature sensor not configured - please add your library!");
    
    #elif defined(SENSOR_TYPE_HUMIDITY)
        // Uncomment and adapt based on your sensor library
        // sensorValue = read_humidity_sensor();
        // Serial.printf("[SENSOR] Humidity: %.2f%%\n", sensorValue);
        Serial.println("[SENSOR] Humidity sensor not configured - please add your library!");
    
    #else
        // Default: read analog
        sensorValue = read_analog_sensor();
        Serial.printf("[SENSOR] Analog reading: %.2f%%\n", sensorValue);
    #endif

    // Publish sensor value
    String payload = "{\"value\":" + String(sensorValue, 2) + "}";
    client.publish(sensorStateTopic.c_str(), payload.c_str(), true);
}

// ==================== Setup ====================
void device_setup() {
    // Initialize topic strings
    sensorStateTopic = String("home/") + String(DEVICE_ID) + String("/state");
    sensorAvailabilityTopic = String("home/") + String(DEVICE_ID) + String("/availability");
    sensorDiscoveryTopic = String("homeassistant/sensor/") + String(DEVICE_ID) + String("/config");
    
    // Build discovery payload
    discoveryPayload = String(R"json({
  "name": )json") + String(SENSOR_NAME) + String(R"json(,
  "uniq_id": )json") + String(DEVICE_ID) + String(R"json(,
  "state_topic": )json") + sensorStateTopic + String(R"json(,
  "availability_topic": )json") + sensorAvailabilityTopic + String(R"json(,
  "unit_of_measurement": "value",
  "value_template": "{{ value_json.value }}",
  "device": {
    "name": )json") + String(DEVICE_NAME) + String(R"json(,
    "identifiers": [")json") + String(DEVICE_ID) + String(R"json("],
    "manufacturer": )json") + String(DEVICE_MANUFACTURER) + String(R"json(,
    "model": )json") + String(DEVICE_MODEL) + String(R"json(,
    "sw_version": )json") + String(DEVICE_VERSION) + String(R"json(
  }
})json");

    pinMode(SENSOR_PIN, INPUT);
    Serial.println("[SENSOR] Device setup...");

    #ifdef AUTO_DISCOVERY
        // Publish discovery config for Home Assistant
        client.publish(sensorDiscoveryTopic.c_str(), discoveryPayload.c_str(), true);
    #endif

    // Publish availability
    client.publish(sensorAvailabilityTopic.c_str(), "online", true);

    // Initial sensor read
    lastSensorRead = millis() - SENSOR_UPDATE_INTERVAL;  // Read immediately on startup
}

// ==================== Loop ====================
void device_loop() {
    // Read sensor at configured interval
    read_sensor();

    // Periodically publish availability status
    static unsigned long lastAvailabilityCheck = 0;
    if (millis() - lastAvailabilityCheck > 30000) {  // Every 30 seconds
        lastAvailabilityCheck = millis();
        client.publish(sensorAvailabilityTopic.c_str(), "online", true);
    }
}

#endif // DEVICE_SENSOR_H
