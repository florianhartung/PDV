#define ENABLE_SERIAL_PLOTTER

constexpr int motor_forward_pin = 3;
constexpr int motor_backward_pin = 9;
constexpr int direction_control_pin = 14;
constexpr int speed_control_pin_from = 4;
constexpr int speed_control_pin_to = 6;

constexpr float acceleration = .01f /*(rad)*s^-2*/;
constexpr int motor_update_interval = 5 /*ms*/;

struct MotorState {
    float speed;        // 0.0 - 255.0
    float target_speed; // 0.0 - 255.0
};

MotorState motor = {0, 0};
unsigned long last_motor_update = 0;
bool force_next_update = false;

void setup() {
    Serial.begin(9600);
    pinMode(motor_forward_pin, OUTPUT);
    pinMode(motor_backward_pin, OUTPUT);

    pinMode(direction_control_pin, INPUT);

    for (int pin = speed_control_pin_from; pin <= speed_control_pin_to; pin++) {
        pinMode(pin, INPUT);
    }
}

void loop() {
    float new_motor_speed = read_new_motor_speed();
    if (motor.target_speed != new_motor_speed) {
        motor.target_speed = new_motor_speed;
        force_next_update = true;
    }

    auto delta_time = millis() - last_motor_update;
    if (delta_time >= motor_update_interval || force_next_update) {
        if (motor.speed != motor.target_speed) {
            update_motor_speed(motor, delta_time);
            change_motor_speed(motor.speed);
        }

        force_next_update = false;
        last_motor_update += delta_time;

#ifdef ENABLE_SERIAL_PLOTTER
        Serial.println("Obergrenze:300\r\nUntergrenze:-300");

        Serial.print("motor_speed:");
        Serial.println(motor.speed);
        Serial.print("target_speed:");
        Serial.println(motor.target_speed);
#endif // ENABLE_SERIAL_PLOTTER
    }
}

int read_new_motor_speed() {
    int new_dip_speed = 0;
    for (int pin = speed_control_pin_from; pin <= speed_control_pin_to; pin++) {
        new_dip_speed <<= 1;
        new_dip_speed |= digitalRead(pin);
    }
    int new_motor_speed = (new_dip_speed * 255) / 7;

    int new_motor_speed_with_direction =
        digitalRead(direction_control_pin) ? -new_motor_speed : new_motor_speed;

    return static_cast<float>(new_motor_speed_with_direction);
}

void update_motor_speed(MotorState& motor, int delta_time) {
    float speed_to = motor.target_speed;
    float speed_from = motor.speed;

    float new_speed = linear_interpolation(speed_from, speed_to, delta_time);
    motor.speed = new_speed;
}

void change_motor_speed(float speed) {
    analogWrite(motor_forward_pin, speed > 0 ? speed : 0);
    analogWrite(motor_backward_pin, speed < 0 ? -speed : 0);
}

/**
 * Linear interpolation with a set slope specified by @link{acceleration}.
 *
 * @param from First interpolation value
 * @param to Second interpolation value
 * @param delta_time Time passed since first interpolation value
 *
 * @returns The interpolated value
 */
float linear_interpolation(float from, float to, int delta_time) {
    float direction = sign(to - from);

    float result = from + direction * (static_cast<float>(delta_time) * acceleration);

    if (direction == 1 && result > to || direction == -1 && result < to) {
        result = to;
    }

    return result;
}

template <typename T>
constexpr int sign(T t) {
    return t < 0 ? -1 : 1;
}
