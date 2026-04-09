#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>

// DS18B20 Configuration
#define ONE_WIRE_BUS 4  // GPIO pin for DS18B20
#define BUZZER_PIN 5    // GPIO pin for buzzer (optional)

// Setup sensors
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
RTC_DS3231 rtc;

// Medicine reminder times (24-hour format)
const int REMINDER_HOUR_1 = 8;   // 8 AM
const int REMINDER_MIN_1 = 0;
const int REMINDER_HOUR_2 = 12;  // 12 PM (noon)
const int REMINDER_MIN_2 = 0;
const int REMINDER_HOUR_3 = 23;  // 10:55 PM
const int REMINDER_MIN_3 = 5;

// Track if reminder already shown
bool reminder1_shown = false;
bool reminder2_shown = false;
bool reminder3_shown = false;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Temperature + Medicine Reminder");
  
  // Setup buzzer (optional)
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Initialize DS18B20
  tempSensor.begin();
  Serial.print("Temperature sensors found: ");
  Serial.println(tempSensor.getDeviceCount());
  
  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1) delay(10);
  }
  
  // Check if RTC lost power and set time if needed
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time...");
    // Set to compile time (adjust this to current time)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Or set manually: rtc.adjust(DateTime(2024, 12, 4, 14, 30, 0));
  }
  
  // Display current time
  DateTime now = rtc.now();
  Serial.print("Current time: ");
  printDateTime(now);
  
  Serial.println("\nMedicine Reminders Set:");
  Serial.println("- 8:00 AM");
  Serial.println("- 12:00 PM");
  Serial.println("- 10:55 PM");
  Serial.println("---------------------------");
}

void loop() {
  // Get current time
  DateTime now = rtc.now();
  
  // Read temperature
  tempSensor.requestTemperatures();
  float tempC = tempSensor.getTempCByIndex(0);
  
  // Display time and temperature
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());
  Serial.print(" | Temp: ");
  
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print(tempC);
    Serial.println("°C");
  } else {
    Serial.println("Error reading temp");
  }
  
  // Check medicine reminders
  checkMedicineReminder(now);
  
  delay(10000);  // Check every 10 seconds
}

void checkMedicineReminder(DateTime now) {
  int currentHour = now.hour();
  int currentMin = now.minute();
  
  // Check 8 AM reminder
  if (currentHour == REMINDER_HOUR_1 && currentMin == REMINDER_MIN_1) {
    if (!reminder1_shown) {
      triggerReminder("8:00 AM");
      reminder1_shown = true;
    }
  } else {
    reminder1_shown = false;  // Reset for next day
  }
  
  // Check 12 PM reminder
  if (currentHour == REMINDER_HOUR_2 && currentMin == REMINDER_MIN_2) {
    if (!reminder2_shown) {
      triggerReminder("12:00 PM");
      reminder2_shown = true;
    }
  } else {
    reminder2_shown = false;  // Reset for next occurrence
  }
  
  // Check 10:55 PM reminder
  if (currentHour == REMINDER_HOUR_3 && currentMin == REMINDER_MIN_3) {
    if (!reminder3_shown) {
      triggerReminder("10:55 PM");
      reminder3_shown = true;
    }
  } else {
    reminder3_shown = false;  // Reset for next day
  }
}

void triggerReminder(String time) {
  Serial.println("\n********************************");
  Serial.println("   🔔 MEDICINE REMINDER! 🔔");
  Serial.print("   Time to take medicine: ");
  Serial.println(time);
  Serial.println("********************************\n");
  
  // Sound buzzer (optional)
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void printDateTime(DateTime dt) {
  Serial.print(dt.year());
  Serial.print('/');
  Serial.print(dt.month());
  Serial.print('/');
  Serial.print(dt.day());
  Serial.print(" ");
  Serial.print(dt.hour());
  Serial.print(':');
  if (dt.minute() < 10) Serial.print('0');
  Serial.print(dt.minute());
  Serial.print(':');
  if (dt.second() < 10) Serial.print('0');
  Serial.println(dt.second());
}

// Function to manually set RTC time
void setRTCTime(int year, int month, int day, int hour, int minute, int second) {
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
  Serial.println("RTC time updated!");
}