#pragma once
#include <Arduino.h>
#include "config/pins.h"

namespace UARTPort {
  void init(uint32_t baudRate);
  bool available();
  String readLine();
}
