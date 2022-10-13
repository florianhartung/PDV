#pragma once

template <typename T>
constexpr size_t array_size(T arr[]) {
  return sizeof(arr) / sizeof(T);
}

template <typename... Ts>
const char* format(const char* format_str, Ts... args) {
  char* buffer;
  asprintf(&buffer, format_str, args...);
  return buffer;
}

template <typename... Ts>
void serial_writef(const char* format_str, Ts... args) {
  Serial.write(format(format_str, args...));
}