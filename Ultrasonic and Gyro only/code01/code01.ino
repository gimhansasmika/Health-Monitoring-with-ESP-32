#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Ultrasonic pins
#define TRIG_PIN 5
#define ECHO_PIN 18

// Buzzer pin
#define BUZZER 15

// Settings
float fallAngle = 30.0;      // degrees
float obstacleDistance = 100; // 1 meter = 100 cm

// ----------------------- Setup -----------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  Serial.println("MPU6050 ready!");
}

// ---------------------- Ultrasonic ----------------------
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;  // cm
  return distance;
}

// ------------------------ Beeps ------------------------
void fallBeep() {
  tone(BUZZER, 1000);  // high tone
  delay(800);
  noTone(BUZZER);
}

void obstacleBeep() {
  tone(BUZZER, 500);
  delay(150);
  noTone(BUZZER);
  delay(150);
}

// ----------------------- Loop -----------------------
void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Get angles (approx)
  float pitch = atan(a.acceleration.x / sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
  float roll  = atan(a.acceleration.y / sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180 / PI;

  float distance = getDistance();

  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print("  Roll: "); Serial.print(roll);
  Serial.print("  Distance: "); Serial.println(distance);

  // ---------- Fall Detection ----------
  if (abs(pitch) > fallAngle || abs(roll) > fallAngle) {
    Serial.println("🚨 FALL DETECTED!");
    fallBeep();
  }

  // ---------- Obstacle Detection ----------
  if (distance > 0 && distance < obstacleDistance) {
    Serial.println("⚠️ Obstacle detected!");
    obstacleBeep();
  }

  delay(100);
}

