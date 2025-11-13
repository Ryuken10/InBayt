#include "WiFiService.h"
#include <WiFi.h>
#include "config/Settings.h"

void WiFiService::connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    // blink small to show activity could be added
    if (millis() - start > 20000) {
      Serial.println("\nRetrying WiFi...");
      start = millis();
    }
  }
  Serial.println("\nWiFi connected, IP: "+ WiFi.localIP().toString());
}
