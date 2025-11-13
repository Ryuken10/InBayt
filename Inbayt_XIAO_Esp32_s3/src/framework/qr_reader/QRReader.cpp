#include "QRReader.h"
#include "drivers/uart/UARTPort.h"

void QRReader::init() {
  UARTPort::init(115200);
}

bool QRReader::available() {
  return UARTPort::available();
}

String QRReader::read() {
  return UARTPort::readLine();
}
