#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_VL53L0X.h>
#include "Config.h"

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();

void initializeSensors() {
    pinMode(XSHUT1_PIN, OUTPUT);
    pinMode(XSHUT2_PIN, OUTPUT);
    pinMode(XSHUT3_PIN, OUTPUT);

    digitalWrite(XSHUT1_PIN, LOW);
    digitalWrite(XSHUT2_PIN, LOW);
    digitalWrite(XSHUT3_PIN, LOW);
    delay(10);
}

int getSensorDistance(int xshutPin, uint8_t i2cAddress) {
    digitalWrite(xshutPin, HIGH);
    delay(10);
    if (!sensor.begin(i2cAddress)) {
        Serial.print("Failed to initialize sensor on XSHUT pin ");
        Serial.println(xshutPin);
        return -1;
    }

    VL53L0X_RangingMeasurementData_t measure;
    sensor.rangingTest(&measure, false);
    digitalWrite(xshutPin, LOW);
    return (measure.RangeStatus != 4) ? measure.RangeMilliMeter : 10000;
}

#endif
