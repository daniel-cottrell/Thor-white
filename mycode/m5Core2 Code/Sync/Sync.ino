#include <WiFi.h>
#include <PubSubClient.h>
#include <M5GFX.h>
#include <Arduino_JSON.h>
#include <assert.h>

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

    //screen display initalise
    display.begin();
    if (display.width() < display.height())
    {
      display.setRotation(display.getRotation() ^ 1);
    }

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

      // snprintf(msg, MSG_BUFFER_SIZE, "{\"type\": 0 , \"category \": 0 , \"ID \": 1 , \"offset\": %lu} ",
      //          3210);  // Format Of string for time sychronisation.
      // // display.print("Publish message: ");
      // // display.println(msg);
      // client.publish("un46986700/computer", msg);  // Publishes a message to the specified topic.
    }
}

void setupWifi() {
    delay(10);
    display.printf("Connecting to %s", ssid);
    WiFi.mode(
        WIFI_STA);  // Set the mode to WiFi station mode.  
    WiFi.begin(ssid, password);  // Start Wifi connection. 

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        display.print(".");
    }
    display.printf("\nSuccess\n");
}

void selectDisplayNum(){

}

void callback(char* topic, byte* payload, unsigned int length) {
    unsigned long arrived = millis();  // Obtain the host startup duration.
    display.print("Message arrived [");
    display.print(topic);
    display.print("] ");
    char input[length];
    for (int i = 0; i < length; i++) {
        display.print((char)payload[i]);
        input[i] = (char)payload[i];
    }
    display.println();

    if(topic == "un46986700/sync"){
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
            // 一旦连接，发送一条消息至指定话题
            client.publish("M5Stack", "hello world");
            // ... and resubscribe.  重新订阅话题
            client.subscribe("M5Stack");
        } else {
            display.print("failed, rc=");
            display.print(client.state());
            display.println("try again in 5 seconds");
            delay(5000);
        }
    }
}
