#include <Wire.h>
#include <iarduino_I2C_Bumper.h>

iarduino_I2C_Bumper bum(0x0C);

int motorRA = 9;
int motorRB = 8;
int motorR_IN1 = 12;
int motorR_IN2 = 13;
int motorL_IN1 = 10;
int motorL_IN2 = 11;
int motorR_EN = 7;
int motorL_EN = 6;

int carSpeed = 150;
int lineThreshold = 1000;

const int buttonPin = 2; 
bool followLineMode = true;

void setup() {
  Serial.begin(9600);
  bum.begin();
  pinMode(motorRA, OUTPUT);
  pinMode(motorRB, OUTPUT);
  pinMode(motorR_IN1, OUTPUT);
  pinMode(motorR_IN2, OUTPUT);
  pinMode(motorL_IN1, OUTPUT);
  pinMode(motorL_IN2, OUTPUT);
  pinMode(motorR_EN, OUTPUT);
  pinMode(motorL_EN, OUTPUT);
  digitalWrite(motorR_EN, HIGH);
  digitalWrite(motorL_EN, HIGH);
  pinMode(buttonPin, INPUT_PULLUP); 
  delay(500);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    followLineMode = !followLineMode;
    delay(500); 
  }

  int sensorValues[8];
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = bum.getLineAnalog(i);
  }

  int linePosition = calculateLinePosition(sensorValues);

  if (followLineMode) {
    int error = linePosition;
    int rightSpeed = constrain(carSpeed + error * 15, 0, 255);
    int leftSpeed = constrain(carSpeed - error * 15, 0, 255);

    setMotor(motorRA, motorR_IN1, motorR_IN2, rightSpeed);
    setMotor(motorRB, motorL_IN1, motorL_IN2, leftSpeed);

    if (linePosition == 0) {
      sharpLeft();
      delay(50);
    }
  } else {
    stop();
    Serial.println("Режим зупинки");
  }

  printSensorData(sensorValues, linePosition);
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

void setMotor(int pwmPin, int in1Pin, int in2Pin, int speed) {
  analogWrite(pwmPin, abs(speed));
  digitalWrite(in1Pin, speed > 0);
  digitalWrite(in2Pin, speed < 0);
}

void forward() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, carSpeed);
  setMotor(motorRB, motorL_IN1, motorL_IN2, carSpeed);
}

void back() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, -carSpeed);
  setMotor(motorRB, motorL_IN1, motorL_IN2, -carSpeed);
}

void left() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, carSpeed);
  setMotor(motorRB, motorL_IN1, motorL_IN2, carSpeed - 30); 
}

void right() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, carSpeed - 30); 
  setMotor(motorRB, motorL_IN1, motorL_IN2, carSpeed);
}

void sharpLeft() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, turnSpeed);
  setMotor(motorRB, motorL_IN1, motorL_IN2, 0);
}

void sharpRight() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, 0);
  setMotor(motorRB, motorL_IN1, motorL_IN2, turnSpeed);
}

void stop() {
  setMotor(motorRA, motorR_IN1, motorR_IN2, 0);
  setMotor(motorRB, motorL_IN1, motorL_IN2, 0);
}

void printSensorData(int sensors[], int position) {
  Serial.print("Position: ");
  Serial.print(position);
  Serial.print(" | Sensors: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(sensors[i]);
    Serial.print(" ");
  }
  Serial.println();
}