#define START_DELAY 250
#define START_BURST 50


enum RobotState {
  Start = 0,
  Straight,
  VeeredLeft,
  VeeredRight,
  AllBlack,
  ObstacleDetected,
  Unexpected
};

void set_state(enum RobotState);

enum RobotState state;
// millisecond timestamp of the last time state changed
long last_time;
// millisecond timestamp of the last pitstop time
long last_pitstop_time;
// light sensor state
int lt;

void setup() {
  state = Start;
  init_robot();
  last_time = millis();
}



void loop() {

  lt = get_lightsensor();
  
  if (state == Start) {
    set_head(90);
    if (delta_time() < START_DELAY) {
      set_left_motor(-5);
      set_right_motor(-5);
    }
    else {
      set_left_motor(100);
      set_right_motor(100);
    }
    if (delta_time() > START_BURST) {
      set_state(Straight);
    }
  }
  else if (state == Straight) {
    set_left_motor(85);
    set_right_motor(85);
    
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

    if (lt == 0b110 || lt == 0b100 || 0b000) {
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
  else if (state == VeeredRight) {
    if (delta_time() < 100) {
      set_left_motor(-5);
      set_right_motor(-5);
    }
    else {
      set_left_motor(191);
      set_right_motor(-102);
    }

    if (lt == 0b110 || lt == 0b100) {
      set_state(VeeredLeft);
    }
    else if (lt == 0b011 || lt == 0b001 || lt == 0b000) {
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
    set_left_motor(-1);
    set_right_motor(-1);
    if (lt = 0b111) {
      set_state(AllBlack);
    }
    else if (get_distance() > 31) {
      set_state(Straight);
    }
  }

  if (get_distance() < 31) {
    set_state(ObstacleDetected);
  }
  
  delay(10);
}

//void set_state(enum RobotState st) {
void set_state(int st) {
  if (state != st && st != Unexpected) {
    state = st;
    last_time = millis();
  }
}

long delta_time() {
  return millis() - last_time;
}
