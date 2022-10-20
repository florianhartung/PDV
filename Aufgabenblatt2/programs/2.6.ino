/**
 * Aufgabe 2.6
 * 
 * Erstellen Sie ein Programm für das STTS-Arduino-System,
 * das abhängig vom Zustand des Tasters die LED mit einer
 * Frequenz von 1 Herz blinkt oder ganz ausschaltet!
 *
 * Vermeiden Sie die Verwendung des „delay()“ Befehls!
 */

#include "digital_pin_util.h"

constexpr static int blink_led_index = 7;

void setup6() {
  pinMode(blink_led_index, OUTPUT);
}

void loop6() {
  // Variablen als statisch deklarieren, dass diese nicht im globalen Namespace liegen aber
  // ihren Wert zwischen Funktionsaufrufen trotzdem behalten
  static bool led_on = false;
  static unsigned long last_toggle_millis;

  unsigned long current_millis = millis();
  
  if(current_millis - last_toggle_millis >= 500) {
    led_on = !led_on;
    led_set(blink_led_index, button_read(2) ? led_on : LOW);
    last_toggle_millis = current_millis;
  }
}
