#include <Servo.h>

Servo s1, s2;

void setup(){

  s1.attach(9);
  s2.attach(10);
  
  Serial.begin(9600);
  
}

float s1_state = 90;
int v0, v1, v2, v3, v4;
void loop() {
  v0 = analogRead(A0);
  v1 = analogRead(A1);
  v2 = analogRead(A2);
  v3 = analogRead(A3);
  v4 = analogRead(A4);


  Serial.print(v0); Serial.print(" ");
  Serial.print(v1); Serial.print(" ");
  Serial.print(v2); Serial.print(" ");
  Serial.print(v3); Serial.print(" ");
  Serial.print(v4); Serial.print(" ");
  Serial.println();
  
  if (v0 > v2 + 20) {
      s1_state += 5;
  }
  else if (v2 > v0 + 20) {
      s1_state -= 5;
  }
  
  s1_state = constrain(s1_state, 0, 180);
  s1.write(s1_state);
  
  delay(1);
  
}
