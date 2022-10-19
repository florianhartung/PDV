/**
 * Aufgabe 2.4
 * 
 * Erstellen Sie ein Programm für das STTS Arduino-System,
 * das eine LED im Sekundentakt blinken lässt.
 * 
 * Verwenden Sie in diesem einfachen Programm die Funktion „delay(msec)“!
 */

#include "digital_pin_util.h"

void setup4() {
  pins_set(0, 15, OUTPUT);
}

int last_index = 0;

void loop4() {
  led_set(last_index, LOW);
  int current_index = (last_index + 1) % 8;
  led_set(current_index, HIGH);
  last_index = current_index;
  delay(50);
}
