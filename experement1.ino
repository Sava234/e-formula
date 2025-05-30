int dig_lineR = 3;
int dig_lineL = 4;

int engR1 = 8;
int engR2 = 9;
int engL1 = 10;
int engL2 = 11;

int carSpeed = 150;
int lowSpeed = 0;  // чтобы машинка не теряла скорость,
                   // во время поворота могут работать оба колеса
                   // вы можете поэкспериментировать со значениями lowSpeed,
                   // поставив 0, 50, 70 или выше, в зависимости от трека

void setup() {
   Serial.begin(9600);
   
   pinMode(dig_lineR, INPUT);
   pinMode(dig_lineL, INPUT);

   pinMode(engR1, OUTPUT);
   pinMode(engR2, OUTPUT);
   pinMode(engL1, OUTPUT);
   pinMode(engL2, OUTPUT);
}
void loop() {
   // функцию back в итоге я предпочёл отключить
   /*if(!digitalRead(dig_lineL)&!digitalRead(dig_lineR)) {
      back();
      delay(0);
   } */
   if(digitalRead(dig_lineL)) {
      left();
      delay(0);
   }
   if(digitalRead(dig_lineR)) {
      right();
      delay(0);
   }
   if(digitalRead(dig_lineL)&digitalRead(dig_lineR)) {
      forward();
      delay(0);
   }
}
void back() {
   analogWrite(engR2, carSpeed-70); // машинка сдает назад с меньшей скоростью,
   analogWrite(engL2, carSpeed-70); // чем едет вперёд
   analogWrite(engR1, 0); 
   analogWrite(engL1, 0); 
}
void left() {
   analogWrite(engL2, 0);
   analogWrite(engR1, carSpeed);
   analogWrite(engR2, 0);
   analogWrite(engL1, lowSpeed); 
}
void right() {
   analogWrite(engR2, 0);
   analogWrite(engL1, carSpeed);
   analogWrite(engR1, lowSpeed);
   analogWrite(engL2, 0);
}
void forward() {
   analogWrite(engR1, carSpeed); 
   analogWrite(engL1, carSpeed);
   analogWrite(engR2, 0);
   analogWrite(engL2, 0);
}
