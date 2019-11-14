#include <Servo.h>

Servo s1, s2;

void setup(){

  s1.attach(9);
  s2.attach(10);
  
  Serial.begin(9600);
  
}

float s1_state = 90;
float s2_state = 90;
int v0, v1, v2, v3, v4;
void loop() {
  v0 = analogRead(A0); //top 
  v1 = analogRead(A1); //bottom
  v2 = analogRead(A2); //right
  v3 = analogRead(A3); //left 
  v4 = analogRead(A4); //middle 

  Serial.print(v0); Serial.print(" ");
  Serial.print(v1); Serial.print(" ");
  Serial.print(v2); Serial.print(" ");
  Serial.print(v3); Serial.print(" ");
  Serial.print(v4); Serial.print(" "); //middle
  Serial.println();
  
  if (v3 > v2 + 20) {
      s1_state += 0.5;
  }
  else if (v2 > v3 + 20) {
      s1_state -= 0.5;
  } 

  if (v4 > 900) {
     delay(1); 
  }


  if (v0 > v1 + 20) {
     s2_state += 0.5;
  } else if (v1 > v0 + 20) {
     s2_state -= 0.5;
  }
  
  s2_state = constrain(s2_state, 90, 180);
  s1_state = constrain(s1_state, 0, 180);
  s1.write(s1_state);
  s2.write(s2_state);
  
  delay(1);
  
}
