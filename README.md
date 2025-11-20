# 🩺 Smart Health Monitoring & Fall Detection System (ESP32)

A smart ESP32-based health monitoring system that measures heart rate, SpO₂, detects falls using the MPU6050, reminds medicine times via the DS3231 RTC, and senses obstacles with an ultrasonic sensor. Designed for elderly care with real-time alerts and safety monitoring.

---

## 🚀 Features
- ❤️ Real-time Heart Rate & SpO₂ (MAX30102)
- 📉 Fall Detection (MPU6050)
- ⏰ Medicine Reminder (DS3231 RTC)
- 📏 Obstacle/Distance Monitoring (Ultrasonic Sensor)
- 🔊 Buzzer Alerts
- 🌐 ESP32 WiFi Support for Cloud/Dashboard

---

## 🔧 Hardware Used
- ESP32 (30-pin)
- MAX30102 Heart Rate & SpO₂ Sensor
- MPU6050 Accelerometer + Gyroscope
- DS3231 RTC Module
- Ultrasonic Sensor (HC-SR04)
- Buzzer + Jumper Wires

---

## 📡 How It Works
1. Sensors send data to the ESP32 via I2C and digital pins.  
2. ESP32 checks health, fall events, medicine time, and distance.  
3. Alerts trigger using a buzzer or optional app/dashboard.  
4. System can be mounted on wheelchairs for elder monitoring.

---

## 📁 Repository Structure
