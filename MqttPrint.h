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

#ifndef __MQTT_PRINT_H__
#define __MQTT_PRINT_H__

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>
#include <stdarg.h>

#define MAX_BUFFER_SIZE 1024

void _mPrint(String message);
#define mprint(val) _mPrint(String(val))
void _mPrintln(String message);
#define mprintln(val) _mPrintln(String(val))
void mprintf(const char *format, ...);
void mbegin(String mqttServer, int mqttPort, String pubTopic);

extern PubSubClient MqttClient;

#endif // __MQTT_PRINT_H__
