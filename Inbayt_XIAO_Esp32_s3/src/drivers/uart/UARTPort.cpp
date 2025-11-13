#include "UARTPort.h"
#include <HardwareSerial.h>

static HardwareSerial QRSerial(1);

void UARTPort::init(uint32_t baudRate) {
  QRSerial.begin(baudRate, SERIAL_8N1, RXD2, TXD2);
}

bool UARTPort::available() {
  return QRSerial.available();
}

String UARTPort::readLine() {
  return QRSerial.readStringUntil('\n');
}
