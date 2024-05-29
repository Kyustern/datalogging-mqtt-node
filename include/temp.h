#ifndef TEMP_H
#define TEMP_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

// #include <Arduino.h>
// using namespace std;

struct AHT_Reading {
    float humidity;
    float temperature;
};

extern void getAhtReading(AHT_Reading* readingPointer);
extern void initAHT();

#endif