#include "DoorRelay.h"
#include "config/pins.h"
#include <Arduino.h>

void DoorRelay::init() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void DoorRelay::open() {
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);
  digitalWrite(RELAY_PIN, LOW);
}
