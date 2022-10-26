#define ENABLE_SERIAL_PLOTTER

/** 
 * Pins zur Ansteuerung des Motors
 */
constexpr int motor_forward_pin = 3;
constexpr int motor_backward_pin = 9;

/**
 * Pin zur Eingabe der angestrebten Motordrehrichtung
 */
constexpr int direction_control_pin = 14;

/**
 * Pins zur Eingabe der angestrebten Motorgeschwindigkeit in Form einer binären Zahl von 0 bis 7
 */
constexpr int speed_control_pin_from = 4;
constexpr int speed_control_pin_to = 6;

/**
 * Beschleunigung mit der der Motor von der aktuellen Geschwindigkeit
 * auf die angestrebte Geschwindigkeit wechselt.
 */
constexpr float acceleration = 0.03f;

/**
 * Zeit, die zwischen der Ansteuerung der Motor-Pins vergeht.
 * Wird benötigt, da die PWM-Pins, welche mit Frequenzen arbeiten,
 * nur funktionieren, wenn zwischen der Ansteuerung dieser eine gewisse Zeit vergeht.
 */
constexpr int motor_update_interval = 5 /*ms*/;

/**
 * Interner Zustand eines Motors.
 * Ein Motor besitzt eine aktuelle Geschwindigkeit, welche an den Ausgabepins
 * ausgegeben wird und eine angestrebte Geschwindigkeit, welche aus den Werten an
 * den Eingabepins ausgelesen wird.
 */
struct MotorState {
    float speed;        // 0.0 - 255.0
    float target_speed; // 0.0 - 255.0
};

/**
 * Der aktuelle, interne Zustand des Motors
 */
MotorState motor = {0, 0};

/**
 * Zeitstempel in ms seit der letzten Aktualisierung des internen Zustandes des Motors
 */
unsigned long last_motor_update = 0;

/**
 * Ein boolean-Wert der genutzt wird, um nach einer erkannten Änderung
 * der Eingaben direkt eine Aktualisierung des Motor-Zustandes auszulösen.
 */
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

/**
 * Lies die aktuellen Zustände der Pins zur Steuerung
 * der Motor-Geschwindigkeit und Richtung ein und gibt diese als eine
 * Ganzzahl zurück.
 *
 * Das Vorzeichen die Richtung und der Betrag die Geschwindigkeit an.
 *
 * @returns Ganzzahl von -255 bis +255
 *
 */
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

/**
 * Akzeptiert den aktuellen Zustand eines Motors als Referenz und ändert/aktualisiert die
 * Geschwindigkeit dessen in Abhängigkeit von der vergangenen Zeit seit der letzten
 * Aktualisierung dieses Wertes.
 *
 * @param motor       Der aktuelle Zustand des Motors als Referenz, dieser wird bearbeitet
 * @param delta_time  Zeit in ms seit der letzen Aktualisierung der Motor-Geschwindigkeit
 */
void update_motor_speed(MotorState& motor, int delta_time) {
    float speed_to = motor.target_speed;
    float speed_from = motor.speed;

    float new_speed = linear_interpolation(speed_from, speed_to, delta_time);
    motor.speed = new_speed;
}

/**
 * Setzt die Zustände der Motor Pins mit einer gegebenen Geschwindigkeit des Motors.
 * Es wird jeweils an einem Pin eine Spannung von 0V und an dem anderen Pin über PWM
 * ein Wert von 0 - 255 angelegt, um die Motorgeschwindigkeit zu setzen.
 *
 * @param speed Geschwindigkeit des Motors von -255.0 bis +255.0,
 *              Richtung gegeben durch das Vorzeichen,
 *              Nachkommastellen werden abgeschnitten
 */
void change_motor_speed(float speed) {
    analogWrite(motor_forward_pin, speed > 0 ? speed : 0);
    analogWrite(motor_backward_pin, speed < 0 ? -speed : 0);
}

/**
 * Lineare Interpolation mit einer konstanten Steigung (hier: @link{acceleration})
 * zwischen zwei Werten in Abhängigkeit von der vergangenen Zeit.
 *
 * @param from        Wert zu Beginn der Messung der vergangenen Zeit
 * @param to          Maximal durch die Interpolation erreichbarer Wert
 * @param delta_time  Vergangene Zeit in ms
 *
 * @returns Ergebnis der Interpolation beider Werte
 */
float linear_interpolation(float from, float to, int delta_time) {
    float direction = sign(to - from);

    float result = from + direction * (static_cast<float>(delta_time) * acceleration);

    if (direction == 1 && result > to || direction == -1 && result < to) {
        result = to;
    }

    return result;
}

/**
 * Vorzeichenfunktion, die -1 oder +1 abhängig
 * vom Vorzeichen des Eingabewerts zurückgibt.
 *
 * @param t Eingabewert
 * @returns -1 oder +1
 */
template <typename T>
constexpr int sign(T t) {
    return t < 0 ? -1 : 1;
}
