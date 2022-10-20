#pragma once

#include "util.h"

#define SUBPROGRAM_FUNCTION_DECLARATION(suffix) \
  void setup##suffix(), loop##suffix();

using setup_function = void (*)();
using loop_function = void (*)();

SUBPROGRAM_FUNCTION_DECLARATION(3);
SUBPROGRAM_FUNCTION_DECLARATION(4);
SUBPROGRAM_FUNCTION_DECLARATION(5);
SUBPROGRAM_FUNCTION_DECLARATION(6);
SUBPROGRAM_FUNCTION_DECLARATION(7);
SUBPROGRAM_FUNCTION_DECLARATION(8);

constexpr static setup_function setup_functions[] = {setup3, setup4, setup5, setup6, setup7, setup8};
constexpr static loop_function loop_functions[] = {loop3, loop4, loop5, loop6, loop7, loop8};

constexpr int num_subprograms = array_size(setup_functions);

void subprogram_setup(size_t setup_index) {
  setup_functions[setup_index]();
}

void subprogram_loop(size_t loop_index) {
  loop_functions[loop_index]();
}
