#define START_BURST 50

enum RobotState {
  Start,
  Straight,
  VeeredLeft,
  VeeredRight,
  AllBlack,
  ObstacleDetected,
  Unexpected
};

enum RobotState state;
long last_time;
long last_pitstop_time;

void setup() {
  state = Start;
  init_robot();
  last_time = millis();

}



void loop() {
  
  if (state == Start) {
    set_left_motor(100);
    set_right_motor(100);
    set_head(90);
    if (delta_time() > START_BURST) {
      set_state(Straight);
    }
  }
  else if (state == Straight) {
    set_left_motor(85);
    set_right_motor(85);
    
    int lt = get_lightsensor();
    if (lt == 0b110 || lt == 0b100) {
      set_state(VeeredLeft);
    }
    else if (lt == 0b011 || lt == 0b001) {
      set_state(VeeredRight);
    }
    else if (lt == 0b010) {
      set_state(Straight);
    }
    else if (lt == 0b111) {
      set_state(AllBlack);
    }
    else {
      set_state(Unexpected);
    }
  }
  else if (state == VeeredLeft) {
    if (delta_time() < 100) {
      set_left_motor(-5);
      set_right_motor(-5);
    }
    else {
      set_left_motor(-102);
      set_right_motor(191);
    }
    int lt = get_lightsensor();
    if (lt == 0b110 || lt == 0b100 || 0b000) {
      // do nothing
      //set_state(VeeredLeft);
    }
    else if (lt == 0b011 || lt == 0b001) {
      set_state(VeeredRight);
    }
    else if (lt == 0b010) {
      set_state(Straight);
    }
    else if (lt == 0b111) {
      set_state(AllBlack);
    }
    else {
      set_state(Unexpected);
    }
  }
  else if (state == VeeredRight) {
    if (delta_time() < 100) {
      set_left_motor(-5);
      set_right_motor(-5);
    }
    else {
      set_left_motor(191);
      set_right_motor(-102);
    }

    int lt = get_lightsensor();
    if (lt == 0b110 || lt == 0b100) {
      set_state(VeeredLeft);
    }
    else if (lt == 0b011 || lt == 0b001 || lt == 0b000) {
      // do nothing
      //set_state(VeeredRight);
    }
    else if (lt == 0b010) {
      set_state(Straight);
    }
    else if (lt == 0b111) {
      set_state(AllBlack);
    }
    else {
      set_state(Unexpected);
    }
  }
  else if (state == AllBlack) {
    if (millis() - last_pitstop_time > 500) {
      set_left_motor(0);
      set_right_motor(0);
      set_head(0);
      while (get_distance() < 31 || delta_time() < 1100) {
        delay(1);
      }
      set_head(90);
      last_pitstop_time = millis();
    }
    set_state(Start);
  }
  else if (state == Unexpected) {
    set_left_motor(0);
    set_right_motor(0);
    for (;;) {
      set_head(0);
      delay(5000);
      set_head(180);
      delay(5000);
    }
  }
  else if (state == ObstacleDetected) {
    set_left_motor(0);
    set_right_motor(0);
    if (get_distance() > 31) {
      set_state(Straight);
    }
  }

  if (get_distance() < 31) {
    set_state(ObstacleDetected);
  }
  
  delay(10);
}

void set_state(enum RobotState st) {
  if (state != st && st != Unexpected) {
    state = st;
    last_time = millis();
  }
}

long delta_time() {
  return millis() - last_time;
}
