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
String fw_box_PubTopic = "";
String fw_box_User = "";
String fw_box_Password = "";
unsigned long fw_box_LastMqttConnectionTime = 0;

void _mPrint(String message)
{
  if (!fw_box_MqttClient.connected()) {
    fw_box_mqttConnect();
  }
  if (fw_box_MqttClient.connected()) {
    fw_box_MqttClient.publish(fw_box_PubTopic.c_str(), message.c_str());
  }
}

void _mPrintln(String message)
{
  if (!fw_box_MqttClient.connected()) {
    fw_box_mqttConnect();
  }
  if (fw_box_MqttClient.connected()) {
    String new_msg = message + "\n";
    fw_box_MqttClient.publish(fw_box_PubTopic.c_str(), new_msg.c_str());
  }
}

void mprintln()
{
  if (!fw_box_MqttClient.connected()) {
    fw_box_mqttConnect();
  }
  if (fw_box_MqttClient.connected()) {
    fw_box_MqttClient.publish(fw_box_PubTopic.c_str(), "\n");
  }
}

void mprintf(const char *format, ...)
{
  if (!fw_box_MqttClient.connected()) {
    fw_box_mqttConnect();
  }
  if (fw_box_MqttClient.connected()) {
    va_list args;
    va_start(args, format);
    char buff[MAX_BUFFER_SIZE];
    vsnprintf(buff, MAX_BUFFER_SIZE, format, args);
    va_end(args);
    fw_box_MqttClient.publish(fw_box_PubTopic.c_str(), buff);
  }
}

void mbegin(String mqttServer, int mqttPort, String pubTopic)
{
  mbegin(mqttServer, mqttPort, pubTopic, "", "");
}

void mbegin(String mqttServer, int mqttPort, String pubTopic, String user, String password)
{
  fw_box_PubTopic = pubTopic;
  fw_box_User = user;
  fw_box_Password = password;
  Serial.println(mqttServer);
  fw_box_MqttClient.setServer(mqttServer.c_str(), mqttPort);
  fw_box_MqttClient.setBufferSize(MAX_BUFFER_SIZE);
  Serial.println("Attempting MQTT connection...");
  fw_box_mqttConnect();
}

void fw_box_mqttConnect()
{
  if (fw_box_LastMqttConnectionTime == 0 || millis() - fw_box_LastMqttConnectionTime > 10 * 1000) {
    // Create a random client ID
    String clientId = "ESP_CLIENT-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (fw_box_User.length() > 0) {
      if (fw_box_MqttClient.connect(clientId.c_str(), fw_box_User.c_str(), fw_box_Password.c_str())) {
        Serial.println("A MQTT connected");
      }
      else {
        Serial.print("A failed, rc=");
        Serial.println(fw_box_MqttClient.state());
      }
    }
    else {
      if (fw_box_MqttClient.connect(clientId.c_str())) {
        Serial.println("B MQTT connected");
      }
      else {
        Serial.print("B failed, rc=");
        Serial.println(fw_box_MqttClient.state());
      }
    }
    fw_box_LastMqttConnectionTime = millis();
  }
}
