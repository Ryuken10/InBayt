#pragma once
#include <Arduino.h>

namespace AWSMQTTClient {
  void connect();
  void reconnect();
  void loop();
  bool isConnected();
  void publishQR(const String &data);
  void messageHandler(char* topic, uint8_t* payload, unsigned int length);
}
