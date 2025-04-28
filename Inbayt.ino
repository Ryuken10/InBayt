#include "Settings.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define AWS_IOT_PUBLISH_TOPIC "board_to_server-inbayt"
#define AWS_IOT_SUBSCRIBE_TOPIC "server_to_boards-inbayt"
#define Relay D8
#define TimeRelay 2000
#define RunLed LED_BUILTIN
#define TimeRunLed 500
#define RS485 D9
#define Red D3
#define TimeRed 1000
#define Blue D5
#define TimeBlue 500
#define Green D4
#define TimeGreen 500
#define EswAnt 3 
#define EAnt 14
#define Rx D7
#define Tx D6

WiFiClientSecure espClient = WiFiClientSecure();
PubSubClient client(espClient);

void Toggle_Relay(void);
void Toggle_RunLed(void);
void Toggle_RedLed(void);
void Toggle_BlueLed(void);
void Toggle_GreenLed(void);
void connectAWS(void);
void sendJsonData(void);
void messageHandler(char* topic, byte* payload, unsigned int length);
void reconnect_AWS(void);
void setup_wifi(void);

void setup() {
  Serial.begin(115200);  // USB serial
  Serial1.begin(115200,SERIAL_8N1,Rx,Tx); // UART0

  pinMode(Relay, OUTPUT);
  pinMode(RunLed, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(EswAnt, OUTPUT);
  pinMode(EAnt, OUTPUT);
  pinMode(RS485, OUTPUT);

  digitalWrite(RunLed, HIGH);
  digitalWrite(Relay, LOW);
  digitalWrite(Red, LOW);
  digitalWrite(Blue, LOW);
  digitalWrite(Green, LOW);
  digitalWrite(EswAnt, LOW);
  digitalWrite(EAnt, HIGH);
  digitalWrite(RS485, LOW);

  setup_wifi();
  connectAWS();
}

void loop() {
  if (Serial1.available()> 0) {
    sendJsonData();
  }

  if (!client.connected()) {
    reconnect_AWS();
  }
  client.loop();
  Toggle_RunLed();
}

void Toggle_RunLed(void)
{
    digitalWrite(RunLed, HIGH);
    delay(TimeRunLed);
    digitalWrite(RunLed, LOW);
}
void Toggle_RedLed(void)
{
    digitalWrite(Red, HIGH);
    delay(TimeRed);
    digitalWrite(Red, LOW);
}
void Toggle_BlueLed(void)
{
    digitalWrite(Blue, HIGH);
    delay(TimeBlue);
    digitalWrite(Blue, LOW);
}
void Toggle_GreenLed(void)
{
    digitalWrite(Green, HIGH);
    delay(TimeGreen);
    digitalWrite(Green, LOW);
}

void Toggle_Relay(void)
{   
    digitalWrite(Relay,HIGH);
    delay(TimeRelay);
    digitalWrite(Relay, LOW);
}

void connectAWS(void) {
  delay(100);
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

  while (!client.connected()) {
    Serial.println("Connecting to AWS....");
    Toggle_BlueLed();
    if (client.connect(THINGNAME)) {
        Serial.println("connected");
        } 
    else {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(200);
      }
    // Subscribe to a topic
    client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  }
}

void sendJsonData() {

  String data = Serial1.readStringUntil('\n');
  Serial.println(data);
  delay(100);
  // Crea un objeto JSON
  StaticJsonDocument<200> doc;
  doc["qr_data"] = data;      // Dato leído del QR
  doc["device"] = "InBayt-01-IN"; // Identificación del dispositivo
  doc["status"] = "OK";  // Estado


  // Serializa el objeto JSON a un buffer
  char buffer[256];
  serializeJson(doc, buffer);

  // Publica el mensaje JSON al tema MQTT
  if (client.publish(AWS_IOT_PUBLISH_TOPIC, buffer)) {
    Serial.println("Mensaje JSON publicado:");
    Serial.println(buffer);
  } else {
    Serial.println("Error al publicar mensaje JSON.");
  }
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("incoming: ");
  Serial.println(topic);
  Toggle_GreenLed(); 
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;  // No seguimos si falló el JSON
  }

  const char* message = doc["message"];

  if (message) {
    Serial.print("Message received: ");
    Serial.println(message);

    if (strcmp(message, "allowed") == 0) {
      Toggle_Relay();
    } 
    else if (strcmp(message, "notAllowed") == 0) {
      Toggle_RedLed();       
      } 
    else {
      Serial.println("Unknown message");
      Toggle_RedLed();
    }
  } 
  else {
    Serial.println("No 'message' field found in JSON");
    Toggle_RedLed();
  }
}

void reconnect_AWS(void) {
  while (!client.connected()) {
    Serial.println("Attempting AWS IoT Core connection...");
    // Subscribe to a topic
    Toggle_BlueLed();
    if (client.connect(THINGNAME)) {
      Serial.println("AWS IoT Connected!");
      client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup_wifi(void) {
  WiFi.mode(WIFI_STA);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    Toggle_GreenLed();
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  espClient.setCACert(AWS_CERT_CA);
  espClient.setCertificate(AWS_CERT_CRT);
  espClient.setPrivateKey(AWS_CERT_PRIVATE);
}
