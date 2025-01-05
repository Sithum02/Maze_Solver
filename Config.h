#ifndef CONFIG_H
#define CONFIG_H

#include <Adafruit_VL53L0X.h>

// Pin definitions
#define XSHUT1_PIN 11
#define XSHUT2_PIN 12
#define XSHUT3_PIN 13

#define MOTOR1_IN1 10
#define MOTOR1_IN2 9
#define MOTOR2_IN1 6
#define MOTOR2_IN2 5

#define ENCODER1_A 2
#define ENCODER1_B 4
#define ENCODER2_A 3
#define ENCODER2_B 7

// Constants
const float WHEEL_DIAMETER = 33.6; // mm
const float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * 3.14159; // mm
const int PULSES_PER_ROTATION = 104;
const float DISTANCE_PER_PULSE = WHEEL_CIRCUMFERENCE / PULSES_PER_ROTATION; // mm per pulse
const float WHEEL_BASE = 84.0; // mm
const float ROTATION_DISTANCE = 3.14159 * WHEEL_BASE / 2.0; // mm for 90-degree turn

#endif
