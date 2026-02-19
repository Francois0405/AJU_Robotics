
void Move_Forward(int car_speed) {
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  analogWrite(5,car_speed);
  analogWrite(6,car_speed);
}

void Move_Backward(int car_speed) {
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  analogWrite(5,car_speed);
  analogWrite(6,car_speed);
}

void Rotate_Left(int car_speed) {
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  analogWrite(5,car_speed);
  analogWrite(6,car_speed);
}

void Rotate_Right(int car_speed) {
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  analogWrite(5,car_speed);
  analogWrite(6,car_speed);
}

void STOP() {
  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  analogWrite(5,0);
  analogWrite(6,0);
}




void setup(){
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
Rotate_Left(150);
delay(500);
// Rotate_Right(150);
// delay(500);
// Move_Forward(150);
// delay(500);
// Move_Backward(150);
// delay(500);

}
