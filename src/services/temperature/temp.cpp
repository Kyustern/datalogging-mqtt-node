// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include <Adafruit_AHTX0.h>

#include <temp.h>

sensors_event_t event;
sensor_t sensor;

uint32_t delayMS;

Adafruit_AHTX0 aht;

void initAHT() {
  if (! aht.begin()) {
    Serial.println("Could not connect to AHT sensor");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
}

// void loop() {
//   sensors_event_t humidity, temp;
//   aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
//   Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
//   Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

//   delay(500);
// }


void getAhtReading(AHT_Reading* readingPointer)
{
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    AHT_Reading current;
    
    float *tempPointer = &(temp.temperature);
    float *humidityPointer = &(humidity.relative_humidity);

    if (isnan(*humidityPointer) || isnan(*tempPointer))
    {
        Serial.println("Error reading sensor output");
        readingPointer->humidity = 0;
        readingPointer->temperature = 0;
    }
    else
    {
        readingPointer->humidity = *humidityPointer;
        readingPointer->temperature = *tempPointer;
    }
}

void aggregate() {
    DynamicJsonDocument doc(1024);
    serializeJson(doc, Serial);
}