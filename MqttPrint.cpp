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

#include "MqttPrint.h"

WiFiClient MqttEspClient39105;
PubSubClient MqttClient39105(MqttEspClient39105);
String PubTopic = "";

void _mPrint(String message)
{
  if (MqttClient39105.connected()) {
    MqttClient39105.publish(PubTopic.c_str(), message.c_str());
  }
}

void _mPrintln(String message)
{
  if (MqttClient39105.connected()) {
    String new_msg = message + "\n";
    MqttClient39105.publish(PubTopic.c_str(), new_msg.c_str());
  }
}

void mprintln()
{
  if (MqttClient39105.connected()) {
    MqttClient39105.publish(PubTopic.c_str(), "\n");
  }
}

void mprintf(const char *format, ...)
{
  if (MqttClient39105.connected()) {
    va_list args;
    va_start(args, format);
    char buff[MAX_BUFFER_SIZE];
    vsnprintf(buff, MAX_BUFFER_SIZE, format, args);
    va_end(args);
    MqttClient39105.publish(PubTopic.c_str(), buff);
  }
}

void mbegin(String mqttServer, int mqttPort, String pubTopic)
{
  PubTopic = pubTopic;
  Serial.println(mqttServer);
  MqttClient39105.setServer(mqttServer.c_str(), mqttPort);
  MqttClient39105.setBufferSize(MAX_BUFFER_SIZE);
  Serial.println("Attempting MQTT connection...");
  // Create a random client ID
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  if (MqttClient39105.connect(clientId.c_str())) {
    Serial.println("connected");
  } else {
    Serial.print("failed, rc=");
    Serial.print(MqttClient39105.state());
  }
}
