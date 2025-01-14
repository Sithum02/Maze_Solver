#include <Wire.h>
#include <Adafruit_VL53L0X.h>

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

volatile int encoder1Count = 0;
volatile int encoder2Count = 0;

Adafruit_VL53L0X sensor = Adafruit_VL53L0X(); // Use a single sensor object

const float WHEEL_DIAMETER = 33.6; // mm
const float WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * 3.14159; // mm
const int PULSES_PER_ROTATION = 105;
const float DISTANCE_PER_PULSE = WHEEL_CIRCUMFERENCE / PULSES_PER_ROTATION; // mm per pulse
const float WHEEL_BASE = 84.0; // mm
const float ROTATION_DISTANCE = 3.14159 * WHEEL_BASE / 2.0; // mm for 90-degree turn

void encoder1ISR_A() {
  if (digitalRead(ENCODER1_A) == digitalRead(ENCODER1_B)) {
    encoder1Count++;
  } else {
    encoder1Count--;
  }
}

void encoder2ISR_A() {
  if (digitalRead(ENCODER2_A) == digitalRead(ENCODER2_B)) {
    encoder2Count++;
  } else {
    encoder2Count--;
  }
}

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

  pinMode(XSHUT1_PIN, OUTPUT);
  pinMode(XSHUT2_PIN, OUTPUT);
  pinMode(XSHUT3_PIN, OUTPUT);

  // Reset all sensors
  digitalWrite(XSHUT1_PIN, LOW);
  digitalWrite(XSHUT2_PIN, LOW);
  digitalWrite(XSHUT3_PIN, LOW);
  delay(10);
  Serial.println("Sensors initialized!");

  attachInterrupt(digitalPinToInterrupt(ENCODER1_A), encoder1ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_A), encoder2ISR_A, CHANGE);

  Serial.begin(9600);
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
  digitalWrite(xshutPin, LOW); // Disable sensor to free memory
  return (measure.RangeStatus != 4) ? measure.RangeMilliMeter : 10000;

}

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

void goForward(int distance_mm, int speedL,int speedR) {
  int targetPulses = distance_mm / DISTANCE_PER_PULSE;
  encoder1Count = 0;
  encoder2Count = 0;

  setMotorSpeed(1, speedL);
  setMotorSpeed(2, speedR);

  while (abs(encoder1Count) < targetPulses || abs(encoder2Count) < targetPulses) {
    // Wait until target pulses are reached
  }

  stopMotors();
}

void turnRight(int speed) {
  int targetPulses = ROTATION_DISTANCE / DISTANCE_PER_PULSE;
  encoder1Count = 0;
  encoder2Count = 0;

  setMotorSpeed(1, speed);
  setMotorSpeed(2, -speed);

  while (abs(encoder1Count) < targetPulses || abs(encoder2Count) < targetPulses) {
    // Wait until target pulses are reached
  }
  stopMotors();
  delay(200);
  goForward(2000, 128,128); // Move forward 30 cm after turning right
  delay(500);
  
}

void turnLeft(int speed) {
  int targetPulses = ROTATION_DISTANCE / DISTANCE_PER_PULSE;
  encoder1Count = 0;
  encoder2Count = 0;

  setMotorSpeed(1, -speed);
  setMotorSpeed(2, speed);

  while (abs(encoder1Count) < targetPulses || abs(encoder2Count) < targetPulses) {
    // Wait until target pulses are reached
  }
  stopMotors();
  delay(200);
  goForward(2000, 128,128); // Move forward 30 cm after turning right
  delay(500);
  
}

// PID constants
float Kp = 1.0;  // Proportional constant
float Ki = 0.0;  // Integral constant
float Kd = 0.0;  // Derivative constant

float previousError = 0;
float integral = 0;

// Function to calculate PID correction
int calculatePID(float error) {
  integral += error;
  float derivative = error - previousError;
  previousError = error;

  // PID output
  int correction = Kp * error + Ki * integral + Kd * derivative;
  return correction;
}

void loop() {

  int distanceL = getSensorDistance(XSHUT1_PIN, 0x30);
  int distanceR = getSensorDistance(XSHUT2_PIN, 0x31);
  int distanceF = getSensorDistance(XSHUT3_PIN, 0x32);

  Serial.print("Left Sensor: ");
  Serial.print(distanceL);
  Serial.print(" mm, Right Sensor: ");
  Serial.print(distanceR);
  Serial.print(" mm, Front Sensor: ");
  Serial.println(distanceF);

  // Calculate the error based on the conditions
  float error;
  if (distanceL < 200 && distanceR < 200) {
    error = distanceL - distanceR;
  } else if (distanceR > 200) {
    error = distanceL - 50;
  } else if (distanceL > 200) {
    error = distanceR - 50;
  } else {
    error = 0;  // Default to no error
  }

  // Calculate PID correction
  int correction = calculatePID(error);

  // Adjust motor speeds based on correction
  int baseSpeed = 128;  // Set a base speed
  int speedLeft = constrain(baseSpeed - correction, -255, 255);
  int speedRight = constrain(baseSpeed + correction, -255, 255);

  

  delay(500);  // Small delay for stability

  //code to run  the robot according to instructions
 

  // goForward(3200,speedLeft,speedRight ); // Move forward 30 cm

  // turnLeft(128);


  

  
}
