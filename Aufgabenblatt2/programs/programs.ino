/**
 * Der Haupteinstiegspunkt für die Aufgaben 2.3 bis 2.8.
 * Dieses Programm ist in mehrere Unterprogramme aufteilt,
 * welche alle gleichzeitig auf den Arduino geladen werden.
 * Es kann dann anschließend direkt am Arduino ein Unterprogramm,
 * welches gestartet werden soll, ausgewählt werden.
 * 
 * Jede Aufgabe besitzt ihr eigenes Unterprogramm, welches sich
 * in der jeweiligen *.ino Datei befindet (2.3 in 2.3.ino, 2.4 in 2.4.ino, ...)
 */

#include "subprogram.h"
#include "util.h"
#include "digital_pin_util.h"

extern const int num_subprograms;

constexpr static int change_subprogram_button_pin = 10;
constexpr static unsigned long exit_selection_mode_button_millis = 2000;

int current_subprogram = 0;

bool subprogram_selection_mode = true;

void setup() {
  start_subprogram_selection_mode();
  draw_subprogram_selection_mode();
}

void loop() {
  if (subprogram_selection_mode) {
    if (digitalRead(change_subprogram_button_pin)) {
      // Zeit, welche der Taster gedrückt wird
      unsigned long button_press_millis = millis();
      while (digitalRead(change_subprogram_button_pin));
      unsigned long button_hold_duration = millis() - button_press_millis;
      delay(20);
      if (button_hold_duration >= exit_selection_mode_button_millis) {
        exit_subprogram_selection_mode();
        subprogram_setup(current_subprogram);
        return;
      }
      step_subprogram_selection_mode();
      draw_subprogram_selection_mode();
    }

  } else {
    if (digitalRead(change_subprogram_button_pin)) {
      start_subprogram_selection_mode();
      delay(500);
      draw_subprogram_selection_mode();
      return;
    }
    subprogram_loop(current_subprogram);
  }
}

void start_subprogram_selection_mode() {
  Serial.end();
  subprogram_selection_mode = true;
  /// TODO: Make sure all dips are off
  pins_set(0, 7, OUTPUT);
  for (int i = 0; i < 8; i++) {
    led_set(i, HIGH);
  }
  while (digitalRead(change_subprogram_button_pin));
}

void exit_subprogram_selection_mode() {
  for (int i = 0; i < 8; i++) {
    led_set(i, HIGH);
  }
  delay(500);
  for (int i = 0; i < 8; i++) {
    led_set(i, LOW);
  }
  pins_set(0, 7, INPUT);  // Pin modi auf Standartwert INPUT zurücksetzen
  subprogram_selection_mode = false;
}

void step_subprogram_selection_mode() {
  // Nächstes Unterprogramm in Auswahl
  current_subprogram = (current_subprogram + 1) % num_subprograms;
}

void draw_subprogram_selection_mode() {
  for (int i = 0; i < 8; i++) {
    led_set(i, i == current_subprogram);
  }
}
