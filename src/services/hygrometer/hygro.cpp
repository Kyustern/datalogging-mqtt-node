#include <Arduino.h>
#include <hygro.h>

uint16_t rawReading;

void initHygro () {
    pinMode(HYGROMETER_PIN, OUTPUT);
}

uint16_t getHygroAverage(int samples, int samplingDelay) {

    // uint16_t readings[samples];
    uint16_t averageSum = 0;

    for (int i = 0; i < samples; i++)
    {
        Serial.print("analogRead(HYGROMETER_PIN) : "); Serial.println(analogRead(HYGROMETER_PIN));
        averageSum = averageSum + analogRead(HYGROMETER_PIN);
        delay(samplingDelay);
        // readings[i] = analogRead(HYGROMETER_PIN);
    }
    
    Serial.print("averageSum / samples : "); Serial.println(averageSum / samples);
    
    return averageSum / samples;

    // return map(rawReading, 0, 1024, 0, 100);
}