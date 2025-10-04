**ESP32 Smart Temperature 🌡️ & Humidity ☁️ Monitor with ThingSpeak**
# 🌤️ ESP32 + DHT22 Wi-Fi Monitor
---

Welcome!
This project uses an ESP32, DHT22 sensor, and a 16x2 I2C LCD to monitor temperature and humidity. It also controls a cooling fan and alert LED based on temperature thresholds, while continuously sending live data to ThingSpeak IoT Cloud for logging and visualization.

---

✨ **Features**

🌡️ Temperature & Humidity Monitoring using DHT22 sensor

📟 16x2 I2C LCD Display for real-time readings

💨 Automatic Fan Control (ON when Temp > 30°C)

🚨 Overheat Alert LED (ON when Temp > 50°C)

📡 Wi-Fi Connectivity with ESP32

☁️ ThingSpeak Cloud Integration (updates every 15 seconds)

---

## 🖥️ Hardware Setup
| Component | ESP32 Pin |
|-----------|-----------|
| DHT22 (Data) | GPIO 4 |
| Fan | GPIO 16 |
| Alert LED | GPIO 17 |
| Status LED | GPIO 5 |
| LCD (SDA/SCL) | 21 / 22 (addr 0x27) |


---

## 🔗 Live Demos
- **Wokwi Simulation:**
   (https://wokwi.com/projects/442685773795056641)
   
- **ThingSpeak Channel:**
  (https://thingspeak.mathworks.com/channels/3083819/private_show)





