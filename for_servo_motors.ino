#include <Wire.h>
#include <iarduino_I2C_Bumper.h>
#include <Servo.h>

iarduino_I2C_Bumper bum(0x0C);

Servo rightServo;
Servo leftServo;

int rightServoPin = 9;
int leftServoPin = 8;

int lineThreshold = 1000;

int minAngle = 0;
int maxAngle = 180;
int centerAngle = 90;

float steeringSensitivity = 1.0;

void setup() {
  Serial.begin(9600);
  bum.begin();
  rightServo.attach(rightServoPin);
  leftServo.attach(leftServoPin);
  rightServo.write(centerAngle);
  leftServo.write(centerAngle);
  delay(500);
}

void loop() {
  int sensorValues[8];
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = bum.getLineAnalog(i);
  }

  int linePosition = calculateLinePosition(sensorValues);

  int steeringAngle = map(linePosition, -40, 40, -90, 90);
  steeringAngle = constrain(steeringAngle, -90, 90);

  int rightServoAngle = centerAngle - (int)(steeringAngle * steeringSensitivity);
  int leftServoAngle = centerAngle + (int)(steeringAngle * steeringSensitivity);

  rightServoAngle = constrain(rightServoAngle, minAngle, maxAngle);
  leftServoAngle = constrain(leftServoAngle, minAngle, maxAngle);

  rightServo.write(rightServoAngle);
  leftServo.write(leftServoAngle);

  printSensorData(sensorValues, linePosition, rightServoAngle, leftServoAngle);
  delay(10);
}

int calculateLinePosition(int sensors[]) {
  int sum = 0;
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (sensors[i] > lineThreshold) {
      sum += (i - 3.5);
      count++;
    }
  }
  return count == 0 ? 0 : round(sum * 10 / count);
}

void printSensorData(int sensors[], int position, int rightAngle, int leftAngle) {
  Serial.print("Position: ");
  Serial.print(position);
  Serial.print(" | Sensors: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(sensors[i]);
    Serial.print(" ");
  }
  Serial.print(" | Right Servo: ");
  Serial.print(rightAngle);
  Serial.print(" | Left Servo: ");
  Serial.print(leftAngle);
  Serial.println();
}