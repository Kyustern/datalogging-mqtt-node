#ifndef AIRMON_H
#define AIRMON_H
#include <DFRobot_ENS160.h>

#define AIRMON_I2C_DATA 
#define AIRMON_I2C_CLOCK

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <temp.h>

struct ENS_Reading
{
    uint8_t aqi;
    uint16_t tvoc;
    uint16_t eco2;
};

extern void initAirmon();
extern void getAirmonReading(ENS_Reading* readingPointer);
extern void setTemperatureCompensation(AHT_Reading* readingPointer);

#endif