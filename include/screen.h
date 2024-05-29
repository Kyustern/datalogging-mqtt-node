#ifndef SCREEN_h
#define SCREEN_h
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include <airmon.h>

extern void initScreen();
extern void drawValues(ENS_Reading* readingPointer);

#endif