/*
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - A1
        Echo: Echo (OUTPUT) - A0
        GND: GND
 */

long distance;
float checkdistance() 
{
  digitalWrite(A1, LOW);
  delayMicroseconds(2);
  digitalWrite(A1, HIGH);
  delayMicroseconds(10);
  digitalWrite(A1, LOW);
  float distance = pulseIn(A0, HIGH) / 58.00;
  delay(10);
  return distance;
} 

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(A1, OUTPUT);
  pinMode(A0, INPUT);
}
 
void loop() {
  distance = checkdistance();
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
