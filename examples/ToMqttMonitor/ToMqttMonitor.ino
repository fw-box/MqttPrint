//
// Copyright (c) 2021 Fw-Box (https://fw-box.com)
// Author: Hartman Hsieh
//
// Description :
//   The function is based on MQTT.
//   When the original serial of ESP8266/ESP32 cannot be used,
//   it can replace serial print.
//
// Libraries :
//

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include "MqttPrint.h"

// Update these with values suitable for your network.

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* MqttServer = "broker.emqx.io";
const char* MessageTopic = "YOUR_MQTT_SUB_TOPIC";

void setup() {

  Serial.begin(115200);
  
  //
  // Begin to connect to thw WiFi AP.
  //
  Serial.println("setup : Attempting to connect...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // Connect to WiFi.

  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (count > 30)
      break;
    delay(500);
    Serial.print(".");
    count++;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  mbegin(MqttServer, 1883, MessageTopic);

}

void loop() {
  mprint("millis = ");
  mprintln(millis());
  mprintln("Hello World");
  mprintf("TEST = %d\n", millis());
  delay(1000);
}
