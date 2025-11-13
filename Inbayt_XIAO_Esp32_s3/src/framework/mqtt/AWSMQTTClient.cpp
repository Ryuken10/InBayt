#include "AWSMQTTClient.h"
#include "config/Settings.h"
#include "drivers/led/StatusLED.h"
#include "drivers/gpio/DoorRelay.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

static WiFiClientSecure secureClient;
static PubSubClient mqttClient(secureClient);

void AWSMQTTClient::connect() {
  secureClient.setCACert(AWS_CERT_CA);
  secureClient.setCertificate(AWS_CERT_CRT);
  secureClient.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, 8883);
  mqttClient.setCallback([](char* topic, byte* payload, unsigned int length){
    AWSMQTTClient::messageHandler(topic, payload, length);
  });

  while (!mqttClient.connected()) {
    Serial.print("Connecting to AWS IoT...");
    StatusLED::blinkBlue();
    if (mqttClient.connect(THINGNAME)) {
      Serial.println(" connected!");
      mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
      StatusLED::blinkGreen();
    } else {
      Serial.print(" failed (state=");
      Serial.print(mqttClient.state());
      Serial.println("), retrying...");
      delay(2000);
    }
  }
}

void AWSMQTTClient::publishQR(const String &data) {
  StaticJsonDocument<256> doc;
  doc["qr_data"] = data;
  doc["device"] = THINGNAME;
  doc["status"] = "OK";
  char buffer[512];
  serializeJson(doc, buffer);
  if (mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, buffer)) {
    Serial.println("Published MQTT message");
  } else {
    Serial.println("MQTT publish failed");
    // try reconnect on failure
    if (!mqttClient.connected()) reconnect();
  }
}

void AWSMQTTClient::messageHandler(char* topic, uint8_t* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(err.c_str());
    return;
  }
  const char* msg = doc["message"]; 
  Serial.print("Received topic: "); Serial.println(topic);
  if (msg) {
    Serial.print("message: "); Serial.println(msg);
    if (strcmp(msg, "allowed") == 0) DoorRelay::open();
    else if (strcmp(msg, "notAllowed") == 0) StatusLED::blinkRed();
    else StatusLED::blinkRed();
  } else {
    Serial.println("No 'message' field");
    StatusLED::blinkRed();
  }
}

void AWSMQTTClient::loop() {
  mqttClient.loop();
}

bool AWSMQTTClient::isConnected() {
  return mqttClient.connected();
}

void AWSMQTTClient::reconnect() {
  connect();
}
