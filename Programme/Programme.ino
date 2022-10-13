#include "function_declarations.h"

#define SUBPROGRAM_COUNT 6
#define CHANGE_SUBPROGRAM_BUTTON_PIN 5

void (*setup_functions[]) ()  = {setup3, setup4, setup5, setup6, setup7, setup8};
void (*loop_functions[]) ()  = {loop3, loop4, loop5, loop6, loop7, loop8};

int current_subprogram = 0;

void setup() {
  start_subprogram(0);
  attachInterrupt(digitalPinToInterrupt(CHANGE_SUBPROGRAM_BUTTON_PIN), onChangeSubProgramButton, FALLING);
}

void loop() {
  loop_functions[current_subprogram]();
}

void onChangeSubProgramButton() {
  current_subprogram += 1;
  if (current_subprogram >= SUBPROGRAM_COUNT) {
    current_subprogram = 0;
  }
}

void start_subprogram(int subprogram_index) {
  setup_functions[subprogram_index]();

  current_subprogram = subprogram_index;
}