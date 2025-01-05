#include "Config.h"
#include "Motors.h"
#include "Encoders.h"
#include "Sensors.h"
#include "MazeSolver.h"

void setup() {
    Serial.begin(9600);

    pinMode(MOTOR1_IN1, OUTPUT);
    pinMode(MOTOR1_IN2, OUTPUT);
    pinMode(MOTOR2_IN1, OUTPUT);
    pinMode(MOTOR2_IN2, OUTPUT);

    pinMode(ENCODER1_A, INPUT_PULLUP);
    pinMode(ENCODER1_B, INPUT_PULLUP);
    pinMode(ENCODER2_A, INPUT_PULLUP);
    pinMode(ENCODER2_B, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER1_A), encoder1ISR_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER2_A), encoder2ISR_A, CHANGE);

    initializeSensors();
    Serial.println("Setup complete.");
}

void loop() {
    floodFillPhase();
    delay(5000);
    fastRunPhase();
    delay(5000);
}
