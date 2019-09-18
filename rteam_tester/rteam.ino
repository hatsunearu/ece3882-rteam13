// Library for controlling the hobby servo
#include <Servo.h>

// Motor driver pin connections
#define ENB 5  // left motor enable
#define IN1 7  // left motor control
#define IN2 8  // left motor control
#define ENA 6  // right motor enable
#define IN3 9  // right motor control
#define IN4 11 // right motor control

// Ultrasonic rangefinder pin connections
#define Echo A4
#define Trig A5

// Servo pins
#define ServoPin 3

// Macros to read the three line tracking sensors.
// These will return '0' with a light surface and '1'
// with a dark surface.
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

// servo object for head
Servo head;

// initialization routine
void init_robot() {
  // Start serial comm in case you want to debug with it
  Serial.begin(9600);

  // Configure the pins that are outputs
  pinMode(Trig, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(ServoPin, OUTPUT);
}

// returns a 3-bit binary number where the MSB is the state of the left sensor
// LSB is the right sensor and the middle bit is the middle sensor
// a set bit (1) = dark surface
int get_lightsensor() {
    return (LT_R == HIGH) | (LT_M == HIGH) << 1 | (LT_L == HIGH) << 2;
}

// prints the status of light sensor to USB Serial
int _test_lightsensor() {
    int lt = get_lightsensor();
    Serial.print("Light Sensor Status: ");
    Serial.print("L: " + (lt & 4) != 0);
    Serial.print("M: " + (lt & 2) != 0);
    Serial.print("R: " + (lt & 1) != 0);
    Serial.println();
}

// {set_left_motor, set_right_motor}
// input: 
// int spd
//      spd has a range of -255 and 255
//      sets the left or right motor at the speed
//      positive speed sets the motor moving forward
//      negative speed sets the motor moving backward
void set_left_motor(int spd) {
  hal_motor(spd < 0, abs(spd), IN1, IN2, ENB);
}
void set_right_motor(int spd) {
  hal_motor(spd < 0, abs(spd), IN4, IN3, ENA);
}

// an hardware abstraction layer for the motor controller
// encapsulates common behavior for left and right motors
// inputs:
// int rev: 0 for forward, 1 for reverse
// int pwm: 0-255: sets speed
// int ctrlpin1, int ctrlpin2
//      Arduino pin numbers
//      for forward, pins are set: ctrlpin1 = HIGH, ctrlpin2 = LOW
//      for reverse, pins are set: ctrlpin1 = LOW, ctrlpin2 = HIGH
// int enpin: Arduino pin number for PWM output
void hal_motor(int rev, int pwm, int ctrlpin1, int ctrlpin2, int enpin) {
  if (pwm == 0) {
    digitalWrite(ctrlpin1, LOW);
    digitalWrite(ctrlpin2, LOW);
    return;
  }
  
  if (!rev) {    
    digitalWrite(ctrlpin2, LOW);
    digitalWrite(ctrlpin1, HIGH);
  }
  else {
    digitalWrite(ctrlpin1, LOW);
    digitalWrite(ctrlpin2, HIGH);
  }
  analogWrite(enpin, pwm);
}

void _test_motor_hal() {
    // forward, max speed, left motor
    hal_motor(0, 255, IN1, IN2, ENB);
    delay(1000);
    // forward, max speed, right motor
    hal_motor(0, 255, IN4, IN3, ENA);
    delay(1000);

    // stop all motors
    hal_motor(0, 0, IN1, IN2, ENB);
    hal_motor(0, 0, IN4, IN3, ENA);
    delay(1000);

    // forward, half speed, left & right motor
    hal_motor(0, 127, IN1, IN2, ENB);
    hal_motor(0, 127, IN4, IN3, ENA);
    delay(1000);

    // forward, half speed, left & right motor
    hal_motor(1, 127, IN1, IN2, ENB);
    hal_motor(1, 127, IN4, IN3, ENA);
    delay(1000);

    hal_motor(0, 0, IN1, IN2, ENB);
    hal_motor(0, 0, IN4, IN3, ENA);
}

void _test_motors() {
    set_left_motor(0);
    set_right_motor(0);

    // ramp up left motor fwd and ramp up right motor rev
    for (int i=0; i<=255; i++) {
        set_left_motor(i);
        set_right_motor(-i);
        delay(5);
    }
    set_left_motor(0);
    set_right_motor(0);

    delay(1000);

    // ramp up left motor rev and ramp up right motor fwd
    for (int i=0; i<=255; i++) {
        set_left_motor(-i);
        set_right_motor(i);
        delay(5);
    }
    set_left_motor(0);
    set_right_motor(0);
}

// moves the servo head to point to an angle
// input: 
// int angle: the angle to set the head at (0-180 degrees)
void set_head(int angle) {
    if (!head.attached()) {
        head.attach(ServoPin);
    }
    head.write(angle);
}

// returns distance from the ultrasonic sensor
// returns: distance in cm
int get_distance() {
  digitalWrite(Trig, LOW);  // ensure ping is off
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH); // start ping
  delayMicroseconds(10); 
  digitalWrite(Trig, LOW);  // end ping
  return (int)pulseIn(Echo, HIGH, 10000) / 58; // measure time of flight and convert to cm
}

void _test_head_servo() {
    // head does not have to be attached, this should work correctly
    head.detach();

    set_head(0);
    delay(1000);
    set_head(90);
    delay(1000);
    set_head(180);
    delay(2000);
}

void _test_distance() {
    Serial.print("Distance: ");
    Serial.print(get_distance());
    Serial.println("cm");
}
