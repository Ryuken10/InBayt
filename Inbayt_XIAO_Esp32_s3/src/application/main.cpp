#include <Arduino.h>
#include "framework/wifi/WiFiService.h"
#include "framework/mqtt/AWSMQTTClient.h"
#include "framework/qr_reader/QRReader.h"
#include "drivers/gpio/DoorRelay.h"
#include "drivers/led/StatusLED.h"
#include "config/pins.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  QRReader::init();
  DoorRelay::init();
  StatusLED::init();

  WiFiService::connect();
  AWSMQTTClient::connect();
}

void loop() {
  if (QRReader::available()) {
    String data = QRReader::read();
    Serial.println("QR read: " + data);
    AWSMQTTClient::publishQR(data);
  }

  if (!AWSMQTTClient::isConnected()) {
    AWSMQTTClient::reconnect();
  }

  AWSMQTTClient::loop();
  StatusLED::blinkRun();
  delay(10);
}
