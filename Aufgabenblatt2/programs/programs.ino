/**
 * Der Haupteinstiegspunkt für die Aufgaben 2.3 bis 2.8.
 * Dieses Programm ist in mehrere Unterprogramme aufteilt,
 * welche alle gleichzeitig auf den Arduino geladen werden können.
 * Es kann dann anschließend direkt am Arduino ein Unterprogramm,
 * welches gestartet werden soll ausgewählt werden.
 * 
 * Jede Aufgabe besitzt ihr eigenes Unterprogramm, welches sich
 * in der jeweiligen *.ino Datei befindet (2.3 in 2.3.ino, 2.4 in 2.4.ino, ...)
 */

#include "subprogram.h"
#include "util.h"
#include "digital_pin_util.h"

extern const int num_subprograms;

constexpr static int change_subprogram_button_pin = 5;

int current_subprogram = 0;

static void on_change_sub_program_button() {
  current_subprogram += 1;
  if (current_subprogram >= num_subprograms) {
    current_subprogram = 0;
  }
}

void setup() {
  //attachInterrupt(digitalPinToInterrupt(change_subprogram_button_pin), on_change_sub_program_button, FALLING);  
  subprogram_setup(2);
  current_subprogram = 2;
}

void loop() {
  subprogram_loop(current_subprogram);
  delay(500);
}