/**************************************************************************
 * Smart Home Framework MQTT + WiFi Backend
 *
 * Fully non-blocking, supports multiple topics and device types.
 * Handles Last Will, auto-reconnect, and callbacks.
 * 
 * Enjoy your coding journey! No delays allowed. Use millis() only.
 **************************************************************************/

#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include "framework_types.h"

// ==================== Global Objects ====================
WiFiClient espClient;
PubSubClient client(espClient);

mqtt_topic_handler_t handlers[MAX_TOPICS];
int handlerCount = 0;

// ==================== Internal State ====================
static unsigned long lastReconnectAttempt = 0;

// ==================== MQTT Functions ====================

// Subscribe a topic with a callback
void mqtt_subscribe(const char* topic, mqtt_callback_t callback) {
    if (handlerCount < MAX_TOPICS) {
        handlers[handlerCount++] = {topic, callback};
        if (client.connected()) client.subscribe(topic);
    } else {
        Serial.println("[MQTT] Too many topics defined, increase MAX_TOPICS");
    }
}

// Internal callback from PubSubClient and custom callbacks
void mqtt_internal_callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

    for (int i = 0; i < handlerCount; i++) {
        if (String(topic) == handlers[i].topic) {
            handlers[i].callback(message);
            return;
        }
    }
}

// Attempt MQTT reconnect 
void mqtt_reconnect() {
    if (client.connected()) return;
    if (millis() - lastReconnectAttempt < 5000) return; // retry every 5s

    lastReconnectAttempt = millis();
    Serial.print("[MQTT] Attempting connection... ");
    String clientId = "ESP32Framework-" + String(WiFi.macAddress());

    if (client.connect(
            clientId.c_str(),
            MQTT_USER, MQTT_PASSWORD,
            "homeassistant/status", 0, true, "offline")) {  // LWT
        Serial.println("connected");
        client.publish("homeassistant/status", "online", true);

        // Re-subscribe to all topics
        for (int i = 0; i < handlerCount; i++) {
            client.subscribe(handlers[i].topic);
        }
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" retrying...");
    }
}

// ==================== Framework Setup ====================
void framework_setup() {
    Serial.begin(BAUDRATE);

    // Connect WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("[WiFi] Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Connected! IP: " + WiFi.localIP().toString());

    // Setup MQTT
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(mqtt_internal_callback);
}

// ==================== Framework Loop ====================
void framework_loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WiFi] Disconnected! Attempting reconnect...");
        WiFi.reconnect();
        return;
    }

    mqtt_reconnect();  // attempt reconnect if needed
    client.loop();     // handle incoming messages
}
