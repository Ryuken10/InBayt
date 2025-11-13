#include "DoorController.h"
#include "framework/mqtt/AWSMQTTClient.h"

void DoorController::sendQR(const String &qr) {
  AWSMQTTClient::publishQR(qr);
}
