#define BLYNK_TEMPLATE_ID "TMPL6tIQC80PE"
#define BLYNK_TEMPLATE_NAME "Smart Elder Care Wheel Chair System"
#define BLYNK_AUTH_TOKEN "-ZDI35kgvD6mcW9XRghJY-c9sI7D_Bbr"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------- WIFI ----------------
char ssid[] = "SLT-4G-45FA";
char pass[] = "16Yal@100";

// ---------------- TEMP SENSOR ----------------
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ---------------- RTC ----------------
RTC_DS3231 rtc;

// ---------------- BUZZER ----------------
#define BUZZER_PIN 18

// ---------------- TIMER ----------------
BlynkTimer timer;

// ---------------- CONTROL ----------------
int lastTriggeredHour = -1;
int lastTriggeredMinute = -1;

// ---------------- BUZZER STATE ----------------
bool buzzerActive = false;
int buzzerCount = 0;
unsigned long buzzerTimer = 0;

// ---------------- TEMPERATURE AVERAGE ----------------
float getTemperature() {
  float sum = 0;

  for (int i = 0; i < 5; i++) {
    sensors.requestTemperatures();
    sum += sensors.getTempCByIndex(0);
    delay(100);
  }

  return sum / 5.0;
}

// ---------------- NON-BLOCKING BUZZER ----------------
void handleBuzzer() {
  if (buzzerActive) {
    if (millis() - buzzerTimer > 300) {
      buzzerTimer = millis();

      if (buzzerCount % 2 == 0) {
        digitalWrite(BUZZER_PIN, HIGH);
      } else {
        digitalWrite(BUZZER_PIN, LOW);
      }

      buzzerCount++;

      if (buzzerCount >= 10) {
        buzzerActive = false;
        digitalWrite(BUZZER_PIN, LOW);
      }
    }
  }
}

// ---------------- SEND DATA ----------------
void sendHealthData() {

  float tempC = getTemperature();
  DateTime now = rtc.now();

  int hour = now.hour();
  int minute = now.minute();

  // ---------------- TEMPERATURE ----------------
  Blynk.virtualWrite(V0, tempC);

  // ---------------- TIME ----------------
  String timeStr = String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute);
  Blynk.virtualWrite(V5, timeStr);

  // ---------------- MEDICINE LOGIC ----------------
  bool trigger = false;

  if ((hour == 16 && minute == 26) ||
      (hour == 21 && minute == 0)) {

    if (hour != lastTriggeredHour || minute != lastTriggeredMinute) {
      trigger = true;
    }
  }

  if (trigger) {

    Serial.println("💊 MEDICINE TIME!");

    // ✅ FINAL MESSAGE FOR BLYNK APP (V4)
    Blynk.virtualWrite(V4, "🚨 TAKE MEDICINE NOW!");

    // START BUZZER
    buzzerActive = true;
    buzzerCount = 0;
    buzzerTimer = millis();

    lastTriggeredHour = hour;
    lastTriggeredMinute = minute;

    // OPTIONAL: event notification (if enabled in Blynk)
    Blynk.logEvent("medicine_alert", "Time to take medicine!");
  }

  else {
    // DON'T OVERWRITE DURING ALERT
    if (!buzzerActive) {
      Blynk.virtualWrite(V4, "System Normal ✔");
    }
  }

  // ---------------- DEBUG ----------------
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | Time: ");
  Serial.println(timeStr);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  // Blynk start
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Sensors
  sensors.begin();

  Wire.begin(21, 22);

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // run once only

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  timer.setInterval(2000L, sendHealthData);

  Serial.println("Health Unit Ready...");
}

// ---------------- LOOP ----------------
void loop() {
  Blynk.run();
  timer.run();
  handleBuzzer();
}