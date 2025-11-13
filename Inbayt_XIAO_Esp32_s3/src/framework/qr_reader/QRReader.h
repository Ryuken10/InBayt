#pragma once
#include <Arduino.h>

namespace QRReader {
  void init();
  bool available();
  String read();
}
