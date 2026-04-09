#include <OneWire.h>
#include <DallasTemperature.h>

// DATA pin
#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  Serial.println("DS18B20 Probe Test");
}

void loop() {
  sensors.requestTemperatures();           // Request temperature
  float tempC = sensors.getTempCByIndex(0); // First sensor on bus

  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC, 2);
    Serial.println(" °C");
  }

  delay(2000); // Read every 2 seconds
}

