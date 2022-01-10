//
// Copyright (c) 2021 Fw-Box (https://fw-box.com)
// Author: Hartman Hsieh
//
// Description :
//   The function is based on MQTT.
//   When the original serial of ESP8266/ESP32 cannot be used,
//   it can replace serial print.
//
//   Install the PC side APP by pip (Python 3.6 up)
//   >pip install MqttMonitor
//   Or update it
//   >pip install MqttMonitor --upgrade
//
// Libraries :
//

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include "MqttPrint.h"

//
// WiFi status LED
// Blink - WiFi is connecting
// Turn on - Connected
// Turn off - Disconnected
//
#define WIFI_STATUS_LED LED_BUILTIN
#if defined(WIFI_STATUS_LED)
  #define STATUS_LED_ON LOW
  #define STATUS_LED_OFF HIGH
#endif // #if defined(WIFI_STATUS_LED)

// Update these with values suitable for your network.

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned long LastWiFiConnectionTime = 0;

const char* MqttServer = "YOUR_MQTT_BROKER";
const char* MqttUser = "YOUR_MQTT_BROKER_USER";
const char* MqttPassword = "YOUR_MQTT_BROKER_PASSWORD";
const char* MessageTopic = "YOUR_MQTT_SUB_TOPIC";

void setup() {

  //Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
#if defined(WIFI_STATUS_LED)
  pinMode(WIFI_STATUS_LED, OUTPUT);
#endif // #if defined(WIFI_STATUS_LED)

  //
  // Begin to connect to thw WiFi AP.
  //
  //Serial.println("WiFi : Attempting to connect...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // Connect to WiFi.

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (count > 30)
      break;
    delay(500);
    //Serial.print(".");
#if defined(WIFI_STATUS_LED)
    if (count % 2) {
      digitalWrite(WIFI_STATUS_LED, STATUS_LED_OFF); // Turn off the LED
    }
    else {
      digitalWrite(WIFI_STATUS_LED, STATUS_LED_ON); // Turn on the LED
    }
#endif // #if defined(WIFI_STATUS_LED)
    count++;
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  
  mbegin(MqttServer, 1883, MessageTopic, MqttUser, MqttPassword);

  mprintln("Begin...");

}

void loop() {
  mprint("millis = ");
  mprintln(millis());
  mprintln("Hello World");
  mprintf("TEST = %d\n", millis());

  if (WiFi.status() == WL_CONNECTED) {
#if defined(WIFI_STATUS_LED)
    digitalWrite(WIFI_STATUS_LED, STATUS_LED_ON); // Turn on the LED
#endif // #if defined(WIFI_STATUS_LED)
  }
  else {
#if defined(WIFI_STATUS_LED)
    digitalWrite(WIFI_STATUS_LED, STATUS_LED_OFF); // Turn off the LED
#endif // #if defined(WIFI_STATUS_LED)
    if (millis() - LastWiFiConnectionTime > 10*1000) {
      //Serial.println("WiFi : Attempting to connect...");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password); // Connect to WiFi.
      LastWiFiConnectionTime = millis();
    }
  }
  
  delay(5000);
}
