/*
**************************************************************************
 * M5Stack Core2 MQTT Communication
 * A program that manages mqtt communication to multiple topics on the same server
 * 
 * Felix Pountney
 * 
 * Changelog:
 * v1.00 = - initial version for the M5Stack-Core2 Shake
 * 
 * Distributed as-is; no warranty is given.
************************************************************************
*/
#include "M5Core2.h"
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// Configure the name and password of the connected wifi for UQ MQTT Serve
const char* ssid        = "infrastructure";
const char* password    = "9uR222dxWCfV";
const char* mqtt_server = "csse4011-iot.zones.eait.uq.edu.au";

// const char* ssid        = "Emmanuel Guest";
// const char* password    = "ioctl-i9s8@KG";
// const char* mqtt_server = "csse4011-iot.zones.eait.uq.edu.au";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
long duration;
int distance;

void setupWifi();
void callback(char* topic, byte* payload, unsigned int length);
void reConnect();
long UltraSonicDuration();

void setup() {
    M5.begin();
    setupWifi();
    client.setServer(mqtt_server,
                     1883);  // Sets the server details.  配置所连接的服务器
    client.setCallback(
        callback);  // Sets the message callback function.  设置消息回调函数

    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    if (!client.connected()) {
        reConnect();
    }
    client.loop();  // This function is called periodically to allow clients to
                    // process incoming messages and maintain connections to the
                    // server.
    // 定期调用此函数，以允许主机处理传入消息并保持与服务器的连接

    unsigned long now =
        millis();  // Obtain the host startup duration.  获取主机开机时长
    if (now - lastMsg > 300) {
        lastMsg = now;
        ++value;
        long dur = UltraSonicDuration();
        int dis = dur/ 58; //calculated from speed of sound in air and adjusted for measured values
        snprintf(msg, MSG_BUFFER_SIZE, "{ \"distance\": %d} ",
                 dis);  // Format to the specified string and store it in MSG.
                          // 格式化成指定字符串并存入msg中
        M5.Lcd.print("Publish message: ");
        M5.Lcd.println(msg);
        client.publish("un46986700/syncoisation",
                       msg);  // Publishes a message to the specified topic.
                              // 发送一条消息至指定话题
        if (value % 12 == 0) {
            M5.Lcd.clear();
            M5.Lcd.setCursor(0, 0);
        }
    }
}

void setupWifi() {
    delay(10);
    M5.Lcd.printf("Connecting to %s", ssid);
    WiFi.mode(
        WIFI_STA);  // Set the mode to WiFi station mode.  设置模式为WIFI站模式
    WiFi.begin(ssid, password);  // Start Wifi connection.  开始wifi连接

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.printf("\nSuccess\n");
}

void callback(char* topic, byte* payload, unsigned int length) {
    M5.Lcd.print("Message arrived [");
    M5.Lcd.print(topic);
    M5.Lcd.print("] ");
    for (int i = 0; i < length; i++) {
        M5.Lcd.print((char)payload[i]);
    }
    M5.Lcd.println();
}

void reConnect() {
    while (!client.connected()) {
        M5.Lcd.print("Attempting MQTT connection...");
        // Create a random client ID.  创建一个随机的客户端ID
        String clientId = "M5Stack-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect.  尝试重新连接
        if (client.connect(clientId.c_str())) {
            M5.Lcd.printf("\nSuccess\n");
            // Once connected, publish an announcement to the topic.
            // 一旦连接，发送一条消息至指定话题
            client.publish("M5Stack", "hello world");
            // ... and resubscribe.  重新订阅话题
            client.subscribe("M5Stack");
        } else {
            M5.Lcd.print("failed, rc=");
            M5.Lcd.print(client.state());
            M5.Lcd.println("try again in 5 seconds");
            delay(5000);
        }
    }
}
