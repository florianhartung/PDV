/**
 * Aufgabe 2.4
 * 
 * Erstellen Sie ein Programm für das STTS Arduino-System,
 * das eine LED im Sekundentakt blinken lässt.
 * 
 * Verwenden Sie in diesem einfachen Programm die Funktion „delay(msec)“!
 */

#include "digital_pin_util.h"

constexpr static int led_index = 7;

void setup4() {
  pinMode(led_index, OUTPUT);
}

void loop4() {
  led_set(led_index, HIGH);
  delay(1000);
  led_set(led_index, LOW);
  delay(1000);
}
