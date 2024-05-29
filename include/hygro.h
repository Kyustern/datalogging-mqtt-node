#ifndef HYGRO_H
#define HYGRO_H

#include <Arduino.h>

#define HYGROMETER_PIN A0

extern void initHygro();
extern uint16_t getHygroAverage(int samples, int samplingDelay);

#endif