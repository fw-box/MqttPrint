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
PubSubClient fw_box_MqttClient(MqttEspClient39105);
String PubTopic = "";
unsigned long fw_box_LastMqttConnectionTime = 0;

void _mPrint(String message)
{
  if (fw_box_MqttClient.connected()) {
    fw_box_MqttClient.publish(PubTopic.c_str(), message.c_str());
  }
}

void _mPrintln(String message)
{
  if (fw_box_MqttClient.connected()) {
    String new_msg = message + "\n";
    fw_box_MqttClient.publish(PubTopic.c_str(), new_msg.c_str());
  }
}

void mprintln()
{
  if (fw_box_MqttClient.connected()) {
    fw_box_MqttClient.publish(PubTopic.c_str(), "\n");
  }
}

void mprintf(const char *format, ...)
{
  if (fw_box_MqttClient.connected()) {
    va_list args;
    va_start(args, format);
    char buff[MAX_BUFFER_SIZE];
    vsnprintf(buff, MAX_BUFFER_SIZE, format, args);
    va_end(args);
    fw_box_MqttClient.publish(PubTopic.c_str(), buff);
  }
}

void mbegin(String mqttServer, int mqttPort, String pubTopic)
{
  PubTopic = pubTopic;
  Serial.println(mqttServer);
  fw_box_MqttClient.setServer(mqttServer.c_str(), mqttPort);
  fw_box_MqttClient.setBufferSize(MAX_BUFFER_SIZE);
  Serial.println("Attempting MQTT connection...");
  // Create a random client ID
  String clientId = "ESP_CLIENT-";
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  if (fw_box_MqttClient.connect(clientId.c_str())) {
    Serial.println("MQTT connected");
  } else {
    Serial.print("failed, rc=");
    Serial.print(fw_box_MqttClient.state());
  }
}

void keepConnecting() {
  if (millis() - fw_box_LastMqttConnectionTime > 20 * 1000) {
    if (!fw_box_MqttClient.connected()) {
      //Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP_CLIENT-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (fw_box_MqttClient.connect(clientId.c_str())) {
        mprintln("MQTT reconnected");
      }
    }
    fw_box_LastMqttConnectionTime = millis();
  }
}
