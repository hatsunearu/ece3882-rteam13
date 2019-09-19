#define SERVO_DELAY 250
#define START_BURST 100


enum RobotState {
  Start = 0,
  StartBurst,
  Straight,
  VeeredLeft,
  VeeredRight,
  AllBlack,
  ObstacleDetected,
  Unexpected
};

void set_state(enum RobotState);

// current robot state
enum RobotState state;
// last state before obstacle was detected
enum RobotState state_before_obstacle;
// millisecond timestamp of the last time state changed
long last_time;
// millisecond timestamp of the last pitstop time
long last_pitstop_time;
// light sensor state
int lt;

void setup() {
  state = Start;
  state_before_obstacle = Start;
  init_robot();
  last_time = millis();
}



void loop() {

  lt = get_lightsensor();
  
  if (state == Start) {
    set_head(90);
    set_left_motor(0);
    set_right_motor(0);

    if (delta_time() > SERVO_DELAY) {
      set_state(StartBurst);
    }
  }
  if (state == StartBurst) {
    set_left_motor(100);
    set_right_motor(100);

    if (delta_time() > START_BURST) {
      set_state(Straight);
    }
  }
  else if (state == Straight) {
    set_left_motor(85);
    set_right_motor(85);
    
    service_linetracking();
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

    service_linetracking();
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

    service_linetracking();
  }
  else if (state == AllBlack) {
    set_left_motor(0);
    set_right_motor(0);
    set_head(0);
    while (get_distance() < 31 || delta_time() < 1100) {
      delay(1);
    }
    set_head(90);
    delay(SERVO_DELAY);
    last_pitstop_time = millis();
    set_state(StartBurst);
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

    // edge case where we hit the pit stop
    // while we are coasting to stop
    if (lt == 0b111) {
      set_state(AllBlack);
    }

    // obstacle distance is above threshold
    // go back to the last state it was in
    else if (get_distance() > 31) {
      set_state(state_before_obstacle);
    }

    // remain in current state
    else {
      set_state(ObstacleDetected);
    }
  }

  // all states can transition to ObstacleDetected using this
  if (get_distance() < 31 && state != ObstacleDetected) {
    // record last state before going into ObstacleDetected
    state_before_obstacle = state; 
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

void service_linetracking() {
  if (lt == 0b110 || lt == 0b100) {
    set_state(VeeredLeft);
  }
  else if (lt == 0b011 || lt == 0b001 || lt == 0b000) {
    set_state(VeeredRight);
  }
  else if (lt == 0b010) {
    set_state(Straight);
  }
  else if (lt == 0b111 && (millis() - last_pitstop_time) > 500) {
    set_state(AllBlack);
  }
  else {
    set_state(Unexpected);
  }
}