#include "StatusLED.h"
#include "config/pins.h"
#include <Arduino.h>

void StatusLED::init() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void StatusLED::blinkGreen() {
  digitalWrite(LED_GREEN, HIGH);
  delay(200);
  digitalWrite(LED_GREEN, LOW);
}

void StatusLED::blinkRed() {
  digitalWrite(LED_RED, HIGH);
  delay(200);
  digitalWrite(LED_RED, LOW);
}

void StatusLED::blinkBlue() {
  digitalWrite(LED_BLUE, HIGH);
  delay(200);
  digitalWrite(LED_BLUE, LOW);
}

void StatusLED::blinkRun() {
  static unsigned long last = 0;
  if (millis() - last > 1000) {
    blinkBlue();
    last = millis();
  }
}
