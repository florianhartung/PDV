#include "setup.h"

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
  subprogram_setup(0);
}

void loop() {
  subprogram_loop(current_subprogram);
}