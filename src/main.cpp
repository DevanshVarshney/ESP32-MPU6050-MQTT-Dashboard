#include <Wire.h>
#include "MPU6050.h"
#include "ILI9341.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define BTN_RESET 15  

// WIFI CREDENTIALS
#define ssid "Wokwi-GUEST"
#define password ""

WiFiClient mqtt;

//MQTT CONFIG
#define MQTT_SERVER "broker.hivemq.com"  // Local MQTT Broker for testing
#define MQTT_PORT 1883
#define MQTT_USER "MQTT_"  //If MQTT is established on cloud
#define MQTT_PASS "MQTt*1234"
PubSubClient client(mqtt);

//MQTT TOPICS FOR LISTENING AND SENDING DATA
const char* PUB_TOPIC = "test/assignment/data";
const char* SUB_TOPIC = "test/assignment/cmd";

//INITIALIZATIONS
MPU6050 mpu;                 
ILI9341 display(5, 16, 17);

unsigned long lastMqttTry = 0;
unsigned long mqttRetryInterval = 1000; //Interval for retrying connection to mqtt
unsigned long mqttSend=0;
unsigned long lastWifiTry = 0;
unsigned long wifiRetryInterval = 1000;
bool wifi_connected = false;
bool mqtt_connected =false;

//Functions Declarations
void checkWifi();
void reconnect();
void disconnectMqtt();
void callback(char* topic, byte *payload, unsigned int length);

void checkWifi(){
  if (WiFi.status() == WL_CONNECTED){
    if (!wifi_connected){
      Serial.println("WiFi Connected!!");
      wifi_connected = true;
      client.setServer(MQTT_SERVER, MQTT_PORT);
      client.setCallback(callback);
    }
  }
  else{
    if (wifi_connected){
      Serial.println("WiFi Disconnected!");
      wifi_connected = false;
      mqtt_connected = false;
    }
  }
}

void reconnect(){
  Serial.println("Connecting to MQTT....");
  if (client.connect("esp32Client")){
    Serial.println("MQTT Connected!!");
    client.subscribe(SUB_TOPIC);
    mqtt_connected = true;
  }
  else{
    Serial.println("Failed!!");
    Serial.println("Retrying!!");
    Serial.print("Failed, rc=");
    Serial.print(client.state());
    Serial.println("");
    mqtt_connected = false;
  }
}


void disconnectMqtt(){
  if (client.connected()){
    client.disconnect();
    Serial.println("MQTT Disconnected!");
  }
  mqtt_connected = false;
} 

void callback(char* topic, byte *payload, unsigned int length){
  Serial.print("Command arrived on Topic : ");
  Serial.println(topic);
  String msg;
  for (unsigned int i=0; i<length; i++){
    msg+=(char)payload[i];
  }
  Serial.printf("Received command : %s\n", msg.c_str());

  if (msg=="RESET"){
    display.clearReadings();
    Serial.println("Display is reset for 1second!!");
    delay(1000);
  }
  else if (msg == "STATUS"){
    display.showStatus(true);
  }
  else if (msg == "DISCONNECT"){
  disconnectMqtt();
}
}

void setup() {
    Wire.begin();
    Serial.begin(115200);
    //WiFI Connection
    Serial.println("Connecting to WiFi!!");
    WiFi.begin(ssid, password);
    
    //RESET BUTTON, MODULES AND DISPLAY INTIALIZATION
    pinMode(BTN_RESET, INPUT_PULLUP);
    mpu.begin();
    display.begin();
    display.drawDashboardLabels();
}

void loop() {
  //CHECK FOR WIFI CONNECTION EVERY 1SEC
  unsigned long currentMillis = millis();
  if (currentMillis - lastWifiTry >= wifiRetryInterval){
    lastWifiTry = currentMillis;
    checkWifi();
  }

  //CHECK FOR MQTT CONNECTION EVERY 1SEC
  if (wifi_connected){
    if (!client.connected()){
      if (currentMillis - lastMqttTry >= mqttRetryInterval){
        lastMqttTry = currentMillis;
        reconnect();
      }
    }
    client.loop();
  }

  //FETCHING THE VALUES FROM MPU 
  float ax = mpu.accelToG(mpu.readAccelX());
  float ay = mpu.accelToG(mpu.readAccelY());
  float az = mpu.accelToG(mpu.readAccelZ());

  float gx = mpu.rotToD(mpu.readGyroX());
  float gy = mpu.rotToD(mpu.readGyroY());
  float gz = mpu.rotToD(mpu.readGyroZ());

  float temp = mpu.tempToC(mpu.readTemp());

  //CLEARING THE SCREEN BASED ON RESET BUTTON
  if (digitalRead(BTN_RESET) == LOW) {
      display.clearReadings();
  }
  else{
    display.showAccel(ax, ay, az);
    display.showGyro(gx, gy, gz);
    display.showTemp(temp);
    display.showStatus(wifi_connected && mqtt_connected);
  }

  //MAKE JSON TO SEND OVER MQTT
  char payload[256];
  snprintf(payload, sizeof(payload),
          "{\"ax\":%.2f,\"ay\":%.2f,\"az\":%.2f,\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,\"temp\":%.2f}",
          ax, ay, az, gx, gy, gz, temp);

  if (millis()-mqttSend>=1000){                 //SEND DATA TO OTHER DEVICE OR CLOUD USING MQTT EVERY 1SEC
    mqttSend = millis();
      if (wifi_connected && mqtt_connected){
        client.publish(PUB_TOPIC, payload);
        Serial.printf("Published : %s\n", payload);
      } 
    }

  delay(2000);  //Refresh rate decider for screen
}
