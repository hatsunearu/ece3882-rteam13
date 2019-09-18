
void setup(){
  init_robot();

  _test_motor_hal();
  delay(1000);
  _test_motors();
  delay(1000);
  _test_head_servo();
}



void loop() {
  _test_lightsensor();
  _test_distance();
}

