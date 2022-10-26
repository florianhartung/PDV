constexpr int motor_forward_pin = 3;
constexpr int motor_backward_pin = 9;

constexpr int direction_control_pin = 14;
constexpr int speed_control_pin_from = 4;
constexpr int speed_control_pin_to = 6;

constexpr float acceleration = .1f/*s^(-1)*/;

constexpr int update_threshold = 5 /*ms*/;

struct MotorState {
  int speed; //0-255
  int target_speed;
};


unsigned long last_millis = 0;
unsigned long last_motor_update = 0;

MotorState current_motor_state = { 0, 0 };


void setup() {
  Serial.begin(9600);
  pinMode(motor_forward_pin, OUTPUT);
  pinMode(motor_backward_pin, OUTPUT);

  pinMode(direction_control_pin, INPUT);

  for(int pin = speed_control_pin_from; pin <= speed_control_pin_to; pin++) {
    pinMode(pin, INPUT);    
  }          
}

void loop() {
  int new_motor_speed = read_new_motor_speed();

  Serial.print("Current: "); Serial.print(current_motor_state.speed); Serial.print("; New: "); Serial.println(new_motor_speed);
  if (current_motor_state.target_speed != new_motor_speed) {
    last_motor_update = millis();
    current_motor_state.target_speed = new_motor_speed;
  }

  // Alle update_threshold millisekunden delayen
  if (current_motor_state.speed != current_motor_state.target_speed) {
    int updated_motor_speed = interpolate_motor_speeds(current_motor_state.speed, current_motor_state.target_speed);
    Serial.print("Updated Motor Speed: "); Serial.println(updated_motor_speed);
    last_motor_update = millis();
    current_motor_state.speed = updated_motor_speed;

    change_motor_speed(current_motor_state.speed);    
  }
  Serial.print("Speed: "); Serial.print(current_motor_state.speed); Serial.print("; Target: "); Serial.println(current_motor_state.target_speed);
  delay(update_threshold);
}

int read_new_motor_speed() {
  int new_dip_speed = 0;
  for (int pin = speed_control_pin_from; pin <= speed_control_pin_to; pin++) {
    new_dip_speed <<= 1;
    new_dip_speed |= digitalRead(pin);
  }
  int new_motor_speed = (new_dip_speed * 255) / 7;
  Serial.println(new_motor_speed);
  return digitalRead(direction_control_pin) ? -new_motor_speed : new_motor_speed;
}

int interpolate_motor_speeds(int speed_from, int speed_to) {
    unsigned long delta_time = millis() - last_motor_update;
    Serial.print("Delta Time: "); Serial.println(delta_time);
    int direction = sign(speed_to - speed_from);
    
    int new_speed = speed_from + direction * (static_cast<float>(delta_time) * acceleration);
      
    if(direction == 1 && new_speed > speed_to || direction == -1 && new_speed < speed_to) {
      return speed_to;
    }

    return new_speed;
}

void change_motor_speed(int speed) {
  Serial.print("Speedo "); Serial.println(speed);
  analogWrite(motor_forward_pin, speed > 0 ? speed : 0);
  analogWrite(motor_backward_pin, speed < 0 ? -speed : 0);
}


template <typename T>
constexpr int sign(T t) {
  return t < 0 ? -1 : 1; 
}
/*
auto current_millis = millis();
  if (current_millis - last_millis >= update_threshold) {
    /*
    last_millis = current_millis;
  }
*/