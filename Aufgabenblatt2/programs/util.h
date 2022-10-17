#pragma once

#define array_size(array) (sizeof(array) / sizeof(*(array)))

template <typename... Ts>
const char* format(const char* format_str, Ts... args) {
  int needed = snprintf(nullptr, 0, format_str, args...) + 1;
  char* buffer = calloc(needed, sizeof(char));
  sprintf(buffer, format_str, args...);
  return buffer;
}

template <typename... Ts>
void serial_writef(const char* format_str, Ts... args) {
  Serial.write(format(format_str, args...));
}