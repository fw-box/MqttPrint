MqttPrint and MqttMonitor
=========================
The function is based on MQTT. When the original serial of ESP8266/ESP32 cannot be used, it can replace serial print.


MqttPrint
---------
A Arduino library
[MqttPrint](https://github.com/fw-box/MqttPrint)

Easy Arduino example:

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


MqttMonitor
-----------
An application to receive the MQTT messages. It's written in Python. Please install it by the command.

    pip install MqttMonitor


Usage
-----
MqttPrint is a MQTT sender, MqttMonitor is a MQTT receiver. They should use the same MQTT broker and sub topic.
This is the screentshot of MqttMonitor.

![alt screenshot](https://github.com/fw-box/MqttPrint/blob/main/screenshot.png?raw=true)
