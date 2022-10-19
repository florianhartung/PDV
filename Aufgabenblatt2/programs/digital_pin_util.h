#pragma once

#include "util.h"

constexpr inline int digital_pin_indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17, 18, 19, 8, 9};

constexpr inline int button_indices[] = { 10, 11, 12, 13 };

constexpr inline size_t button_count = array_size(button_indices);

constexpr inline size_t digital_pin_count = array_size(digital_pin_indices);

enum Group {
  lower = 0b1111 << 0,
  upper = 0b0111 << 4,
  all = lower | upper
};

void pins_set(size_t first, size_t last, int status) {
  for(int i = first; i <= last; i++) {
    pinMode(digital_pin_indices[i], status);
  }
}

void led_set(size_t index, int status) {
  digitalWrite(digital_pin_indices[index], status);
}

int dip_read(size_t index) {
  return digitalRead(digital_pin_indices[index]);
}

int button_read(size_t index) {
  return digitalRead(button_indices[index]);
}

int dip_read_word(Group group) {
  int word = 0;
  for(int i = 0; i < digital_pin_count; i++) {
    word |= dip_read(i) << (digital_pin_count - i - 1); //Schalter 1-1 ist most significant bit, deshalb die Subtraktion digital_pin_count - i - 1
  }
  return word & group;
}