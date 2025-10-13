# MQTT Publisher using ESP32 (PlatformIO + Wokwi)

This project demonstrates how to connect an **ESP32** to an **MQTT broker** and publish/subscribe to topics in real time.  
I built and tested everything using **PlatformIO** in VS Code and simulated the hardware setup on **Wokwi**.

---

## 🔧 Project Overview

This ESP32-based sensor monitoring system displays real-time data from an MPU6050 IMU sensor on an ILI9341 screen. The system uses non-blocking architecture to ensure continuous display updates regardless of connectivity status.
Key Features:

Independent Operation: Display updates continuously with real-time accelerometer, gyroscope, and temperature readings, even without WiFi/MQTT connection
Dynamic Connectivity: WiFi and MQTT connection attempts every 1 second (configurable intervals)
Status Indicator: Visual LED shows connection state - green when connected, red when disconnected
Remote Commands: Supports MQTT commands (RESET, STATUS, DISCONNECT) for remote control and testing
Data Publishing: Publishes sensor data in JSON format every 1 second when connected to MQTT
Hardware Reset: Physical button for manual display clearing

The system prioritizes local functionality while maintaining optional cloud connectivity, using millis() timing to avoid blocking operations.

I used my **local Mosquitto broker (running on Windows)** to visualize messages and commands through the terminal.

---

## ⚙️ Setup Details

### 🧩 Hardware / Simulation
- **ESP32 Dev Module**
- **MPU6050 Sensor** (for acceleration, rotation, and temperature)
- **ILI9341 LCD Display** (to show live readings)
- **Push Button** (for reset)
- **Wokwi** for simulation (no physical board required)

### 💻 Software & Tools
- **PlatformIO (VS Code Extension)** – for building and uploading firmware  
- **Wokwi Simulator** – for virtual ESP32 + sensor testing  
- **Mosquitto MQTT Broker (Windows)** – for local topic publishing/subscription  
- **PubSubClient Library** – to handle MQTT on ESP32

---

## 📡 MQTT Configuration

- **Broker:** Public MQTT Broker is used ("broker.hivemq.com") 
- **Port:** `1883`  
- **Publisher Topic:** `test/assignment/data`  
- **Subscriber Topic:** `test/assignment/cmd`
**If going to cloud MQTT then username and password should be given to the appropiate variables AND add them into the client.setServer(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS)**
- **USERNAME : ** #define MQTT_USER "MQTT_"  
- **PASSWORD : ** #define MQTT_PASS "MQTt*1234"

The ESP32 connects to WiFi, then to the MQTT broker.  
You can view the published data and send commands directly through the Mosquitto terminal.

Example terminal commands:
```bash
# To Publish
mosquitto_pub -h broker.hivemq.com -t "test/assignment/cmd" -m "RESET"
# To Subscribe
mosquitto_sub -h broker.hivemq.com -t "test/assignment/data" -v


# To publish a command
mosquitto_pub -h <your_local_ip> -t esp32/commands -m "RESET"
