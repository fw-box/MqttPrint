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

WiFiClient MqttEspClient;
PubSubClient MqttClient(MqttEspClient);
String PubTopic = "";

void _mPrint(String message)
{
  MqttClient.publish(PubTopic.c_str(), message.c_str());
}

void _mPrintln(String message)
{
  String new_msg = message + "\n";
  MqttClient.publish(PubTopic.c_str(), new_msg.c_str());
}

void mprintf(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  #define MAX_BUFFER_SIZE 512
  char buff[MAX_BUFFER_SIZE];
  sprintf(buff, format, args);
  va_end(args);
  MqttClient.publish(PubTopic.c_str(), buff);
}

void mbegin(String mqttServer, int mqttPort, String pubTopic)
{
  PubTopic = pubTopic;
  Serial.println(mqttServer);
  MqttClient.setServer(mqttServer.c_str(), mqttPort);
  MqttClient.setBufferSize(512);
  Serial.println("Attempting MQTT connection...");
  // Create a random client ID
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  if (MqttClient.connect(clientId.c_str())) {
    Serial.println("connected");
  } else {
    Serial.print("failed, rc=");
    Serial.print(MqttClient.state());
  }
}
