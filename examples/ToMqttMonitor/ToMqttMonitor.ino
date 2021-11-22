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

  //Serial.begin(115200);
  
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
    count++;
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  
  mbegin(MqttServer, 1883, MessageTopic);

  mprintln("Begin...");

}

void loop() {
  mprint("millis = ");
  mprintln(millis());
  mprintln("Hello World");
  mprintf("TEST = %d\n", millis());

  keepConnecting();
  
  delay(5000);
}

void keepConnecting() {
  unsigned long LastWiFiConnectionTime = 0;
  unsigned long LastMqttConnectionTime = 0;
  if (WiFi.status() == WL_CONNECTED) {
    // Loop until we're reconnected
    if (!MqttClient.connected() && (millis() - LastMqttConnectionTime > 10000)) {
      //Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (MqttClient.connect(clientId.c_str())) {
        //Serial.println("MQTT connected");
      } else {
        //Serial.print("failed, rc=");
        //Serial.print(MqttClient.state());
        //Serial.println(" try again in 10 seconds");
        LastMqttConnectionTime = millis();
      }
    }
  }
  else {
    if (millis() - LastWiFiConnectionTime > 10000) {
      //Serial.println("WiFi : Attempting to connect...");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password); // Connect to WiFi.
      LastWiFiConnectionTime = millis();
    }
  }
}
