#include "M5Core2.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5GFX.h>
#include <Arduino_JSON.h>
#include <assert.h>
#include <string.h>

M5GFX display;

WiFiClient espClient;
PubSubClient client(espClient);

//-------------------- UQ MQTT Server --------------------------------
const char* ssid        = "infrastructure";
const char* password    = "9uR222dxWCfV";
const char* mqtt_server = "csse4011-iot.zones.eait.uq.edu.au";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (100)
char msg[MSG_BUFFER_SIZE];
int frame_num = 0;
int frame_length = 1000;
int displayNum = 1;
unsigned long time_offset = 0;
int accel_send = 0;
int gyro_send = 0; 

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

//image_names
extern const unsigned char image1[];
extern const unsigned char image2[];
extern const unsigned char image3[];
extern const unsigned char image4[];
extern const unsigned char image5[];
extern const unsigned char image6[];
extern const unsigned char image7[];
extern const unsigned char image8[];
extern const unsigned char image9[];
int i = 0;
int j = 0;

void setupWifi();
void callback(char* topic, byte* payload, unsigned int length);
void reConnect();


void setup() {

    setupWifi();
    client.setServer(mqtt_server,
                     1883);  // Sets the server details.
    client.setCallback(
        callback);  // Sets the message callback function.

    Serial.begin(9600);

    //screen display initalise
    display.begin();
    if (display.width() < display.height())
    {
      display.setRotation(display.getRotation() ^ 1);
    }

    // selectDisplayNum();

    unsigned long now = millis();  // Obtain the startup duration.

    //time synconsiation to dispaly 1
    if(displayNum != 1){
      snprintf(msg, MSG_BUFFER_SIZE, "{\"type\":0,\"ID\":%d,\"timestamp\":%lu}",
                 displayNum, now);  // Format of string for time sychronisation.
      client.publish("un46986700/sync", msg);  // Publishes a message to the specified topic.
      display.drawString("Syncing time for display", 0, 0);
      while(time_offset == 0){
        if (!client.connected()) {
        reConnect();
        }
        client.loop();  // This function is called periodically to check for response
        if ( (now - millis()) > 1000) {
          now = millis(); 
          display.drawString(".", 1, 0);
        }
      }
      display.clear();
    }
    
    M5.IMU.Init();

}

void loop() {
    if (!client.connected()) {
        reConnect();
    }
    client.loop();  // This function is called periodically to allow clients to
                    // process incoming messages and maintain connections to the
                    // server.

    unsigned long now = millis();  // Obtain the host startup duration.
    if ( (now + time_offset)/frame_length > frame_num) {
      frame_num = ((now + time_offset)/frame_length)%9 +1;
      if(frame_num == 1){
        display.drawJpg(image1, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 2){
        display.drawJpg(image2, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 3){
        display.drawJpg(image3, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 4){
        display.drawJpg(image4, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 5){
        display.drawJpg(image5, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 6){
        display.drawJpg(image6, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 7){
        display.drawJpg(image7, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 8){
        display.drawJpg(image8, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65 // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }else if(frame_num == 9){
        display.drawJpg(image9, ~0u, -1*i*display.width(), -1*j*display.height(),
            3*display.width()  // Width
          , 3*display.height() // Height
          , 0    // X offset
          , 0    // Y offset
          , 1.65  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
          , -1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
          );
      }
      
      display.setTextDatum(textdatum_t::top_left);
      display.drawString("new frame", 0, 0);

      if(accel_send){
        JSONVar accel_message;
        JSONVar accel_xyz;

        accel_message["type"] = 0;
        accel_message["category"] = 3;
        accel_message["ID"] = displayNum;
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        accel_xyz[0] = accX;
        accel_xyz[1] = accY;
        accel_xyz[2] = accZ;
        accel_message["accel"] = accel_xyz;
        client.publish("un46986700/computer", JSON.stringify(accel_message).c_str());  // Publishes a message to the specified topic.
      }

      if(gyro_send){
        JSONVar gyro_message;
        JSONVar gyro_xyz;

        gyro_message["type"] = 0;
        gyro_message["category"] = 4;
        gyro_message["ID"] = displayNum;
        M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
        gyro_xyz[0] = gyroX;
        gyro_xyz[1] = gyroY;
        gyro_xyz[2] = gyroZ;
        gyro_message["gyro"] = gyro_xyz;
        client.publish("un46986700/computer", JSON.stringify(gyro_message).c_str());  // Publishes a message to the specified topic.
      }

    }

    //check for shake
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    if (accX > 80 || accY > 80 || accZ > 80) {
      M5.Axp.SetVibration(true);  // Open the vibration.
      delay(100);
      M5.Axp.SetVibration(false);
    }

    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    if (accX > 100 || accY > 100 || accZ > 100) {
      M5.Axp.SetVibration(true);  // Open the vibration.
      delay(100);
      M5.Axp.SetVibration(false);
    }

}

void setupWifi() {
    delay(10);
    display.fillScreen(RED);
    display.printf("Connecting to %s", ssid);
    WiFi.mode(
        WIFI_STA);  // Set the mode to WiFi station mode.  
    WiFi.begin(ssid, password);  // Start Wifi connection. 

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        display.print(".");
    }
    display.fillScreen(WHITE);
}

void selectDisplayNum(){
  display.clearDisplay();
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      display.fillSmoothRoundRect(
        (display.width()/20 + i*display.width()/3)
        , (display.height()/20 + j*display.height()/3)
        , display.width()/3.5
        , display.height()/4
        , 10
        , BLUE
      );
    }
  }
  while(displayNum == 0){
    M5.update();
    display.setCursor(0,0);
    if(!M5.Touch.ispressed()) {
      continue;
    }
    TouchPoint_t coordinate;
    coordinate = M5.Touch.getPressPoint();
    snprintf(msg, MSG_BUFFER_SIZE, "X: %d Y:%d",
                 coordinate.x, coordinate.y);  // Format of string for time sychronisation.
    display.print(msg);
    if(coordinate.x < 0 || coordinate.y < 0){
      continue;
    }
    i = (int) coordinate.x/ (display.width()/3);
    j = (int) coordinate.y/ (display.width()/3);
    displayNum = ((int) coordinate.x/ (display.width()/3)) + 3*((int) coordinate.y/ (display.width()/3)) + 1;
  }
  display.setCursor(0,0);
  snprintf(msg, MSG_BUFFER_SIZE, "Selected Num %d",
                 displayNum);  // Format of string for time sychronisation.
  display.println(msg);
  delay(3000);
  
}

void callback(char* topic, byte* payload, unsigned int length) {
    unsigned long arrived = millis();  // Obtain the host startup duration.
    char input[length];
    for (int i = 0; i < length; i++) {
        input[i] = (char)payload[i];
    }

    if(strcmp(topic,"un46986700/sync") == 0){
      JSONVar message = JSON.parse(input);
      if (!message.hasOwnProperty("type")){
        return;
      }
      if((int) message["type"] == 0 && displayNum == 1){
        JSONVar response;
        JSONVar timestamps;

        response["type"] = 0;
        response["ID"] = message["ID"];
        timestamps[0] = message["timestamp"];
        timestamps[1] = arrived;
        timestamps[2] = millis();
        response["timestamps"] = timestamps;
        client.publish("un46986700/sync", JSON.stringify(response).c_str());  // Publishes a response to sync topic.
      }
      if((int) message["type"] == 0 && (int) message["ID"] == displayNum){
        JSONVar timestamps = message["timestamps"];
        unsigned long trans_delay = ((arrived - (unsigned long) timestamps[0]) - ( (unsigned long) timestamps[2] - (unsigned long) timestamps[1]))/2;
        time_offset = (unsigned long) timestamps[1] - trans_delay - (unsigned long) timestamps[0];
      }
    }else if(strcmp(topic,"un46986700/toggle") == 0){
      JSONVar message = JSON.parse(input);
      if (!message.hasOwnProperty("toggle")){
        return;
      }
      if ((int) message["toggle"] == displayNum){
        gyro_send = !gyro_send;
        accel_send = !accel_send;
      }
    }
    

}

void reConnect() {
    while (!client.connected()) {
        display.print("Attempting MQTT connection...");
        // Create a random client ID.  创建一个随机的客户端ID
        String clientId = "M5Stack-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect.  尝试重新连接
        if (client.connect(clientId.c_str())) {
            display.printf("\nSuccess\n");
            // Once connected, publish an announcement to the topic.
            // ... and resubscribe.  重新订阅话题
            client.subscribe("un46986700/#");
        } else {
            display.print("failed, rc=");
            display.print(client.state());
            display.println("try again in 5 seconds");
            delay(5000);
        }
    }
}
