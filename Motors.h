#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "Config.h"

void setMotorSpeed(int motor, int speed) {
    int in1, in2;

    if (motor == 1) {
        in1 = MOTOR1_IN1;
        in2 = MOTOR1_IN2;
    } else if (motor == 2) {
        in1 = MOTOR2_IN1;
        in2 = MOTOR2_IN2;
    } else {
        return;
    }

    speed = constrain(speed, -255, 255);

    if (speed > 0) {
        analogWrite(in1, speed);
        analogWrite(in2, 0);
    } else if (speed < 0) {
        analogWrite(in1, 0);
        analogWrite(in2, -speed);
    } else {
        analogWrite(in1, 0);
        analogWrite(in2, 0);
    }
}

void stopMotors() {
    setMotorSpeed(1, 0);
    setMotorSpeed(2, 0);
}

#endif
