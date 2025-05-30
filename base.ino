#include <Wire.h>
#include <iarduino_I2C_Bumper.h>

// Пин подключения для I2C бампера (если используется)
iarduino_I2C_Bumper bum(0x0C); // Укажите правильный адрес

// Пины для управления моторами
const int motorR_EN = 5;   // Enable правого мотора (PWM)
const int motorR_IN1 = 6;  // IN1 правого мотора
const int motorR_IN2 = 7;  // IN2 правого мотора

const int motorL_EN = 10;  // Enable левого мотора (PWM)
const int motorL_IN1 = 8;  // IN1 левого мотора
const int motorL_IN2 = 9;  // IN2 левого мотора

// Настройки скорости
int carSpeed = 150;      // Базовая скорость (0-255)
int lowSpeed = 70;       // Скорость при повороте
int turnSpeed = 200;     // Скорость при резком повороте
int lineThreshold = 1000; // Порог для определения линии

// Время последнего действия
unsigned long lastActionTime = 0;
const unsigned long actionTimeout = 1000; // 1 секунда бездействия

void setup() {
  Serial.begin(9600);
  
  // Инициализация бампера (если используется)
  if (!bum.begin()) {
    Serial.println("Бампер не найден!");
    while(1);
  }

  // Настройка пинов моторов
  pinMode(motorR_EN, OUTPUT);
  pinMode(motorR_IN1, OUTPUT);
  pinMode(motorR_IN2, OUTPUT);
  pinMode(motorL_EN, OUTPUT);
  pinMode(motorL_IN1, OUTPUT);
  pinMode(motorL_IN2, OUTPUT);

  // Остановка моторов при старте
  stop();
  delay(500);
}

void loop() {
  // Если есть данные от Serial - обработать команду
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    handleSerialCommand(command);
    lastActionTime = millis();
  }
  
  // Если прошло много времени без команд - перейти в автономный режим
  if (millis() - lastActionTime > actionTimeout) {
    autonomousDrive();
  }
}

// Автономное движение по линии
void autonomousDrive() {
  int sensorValues[8];
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = bum.getLineAnalog(i);
  }

  int linePosition = calculateLinePosition(sensorValues);

  // Управление в зависимости от положения линии
  if (linePosition == 0) {
    stop();
  } else if (linePosition < -2) {
    sharpRight();
  } else if (linePosition < -1) {
    right();
  } else if (linePosition > 2) {
    sharpLeft();
  } else if (linePosition > 1) {
    left();
  } else {
    forward();
  }

  // Отправка данных на Serial
  printSensorData(sensorValues, linePosition);
  delay(10);
}

// Расчет положения линии
int calculateLinePosition(int sensors[]) {
  int sum = 0;
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (sensors[i] > lineThreshold) {
      sum += (i - 3.5); // Центрируем вокруг 0
      count++;
    }
  }
  return count == 0 ? 0 : round(sum * 10 / count);
}

// Управление одним мотором
void setMotor(int enPin, int in1Pin, int in2Pin, int speed) {
  analogWrite(enPin, abs(speed));
  digitalWrite(in1Pin, speed > 0 ? HIGH : LOW);
  digitalWrite(in2Pin, speed > 0 ? LOW : HIGH);
}

// Команды движения
void forward() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, carSpeed);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, carSpeed);
}

void back() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, -carSpeed);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, -carSpeed);
}

void left() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, carSpeed);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, lowSpeed);
}

void right() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, lowSpeed);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, carSpeed);
}

void sharpLeft() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, turnSpeed);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, 0);
}

void sharpRight() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, 0);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, turnSpeed);
}

void stop() {
  setMotor(motorR_EN, motorR_IN1, motorR_IN2, 0);
  setMotor(motorL_EN, motorL_IN1, motorL_IN2, 0);
}

// Вывод данных датчиков
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

// Обработка команд с Serial
void handleSerialCommand(String command) {
  if (command == "FORWARD") {
    forward();
  } else if (command == "BACK") {
    back();
  } else if (command == "LEFT") {
    left();
  } else if (command == "RIGHT") {
    right();
  } else if (command == "SHARP_LEFT") {
    sharpLeft();
  } else if (command == "SHARP_RIGHT") {
    sharpRight();
  } else if (command == "STOP") {
    stop();
  } else if (command == "GET_SETTINGS") {
    Serial.print("SETTINGS:");
    Serial.print(carSpeed);
    Serial.print(",");
    Serial.print(lowSpeed);
    Serial.print(",");
    Serial.print(turnSpeed);
    Serial.print(",");
    Serial.println(lineThreshold);
  } else if (command.startsWith("SET_SETTINGS:")) {
    // Разбираем настройки
    int firstComma = command.indexOf(',');
    int secondComma = command.indexOf(',', firstComma + 1);
    int thirdComma = command.indexOf(',', secondComma + 1);
    
    if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
      carSpeed = command.substring(13, firstComma).toInt();
      lowSpeed = command.substring(firstComma + 1, secondComma).toInt();
      turnSpeed = command.substring(secondComma + 1, thirdComma).toInt();
      lineThreshold = command.substring(thirdComma + 1).toInt();
      
      Serial.print("SETTINGS_UPDATED:");
      Serial.print(carSpeed);
      Serial.print(",");
      Serial.print(lowSpeed);
      Serial.print(",");
      Serial.print(turnSpeed);
      Serial.print(",");
      Serial.println(lineThreshold);
    }
  }
}