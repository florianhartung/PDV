#pragma once

#define array_size(array) (sizeof(array) / sizeof(*(array)))
 
template <typename... Ts>
char* format(const char* format_str, Ts... args) {
  static char* buffer;
  static bool buffer_has_been_initialized;
  
  int needed = snprintf(nullptr, 0, format_str, args...) + 1;
  
  if(!buffer_has_been_initialized) {
    buffer = calloc(needed, sizeof(char));
    buffer_has_been_initialized = true;
  } else {
    buffer = realloc(buffer, needed);
  }
  
  sprintf(buffer, format_str, args...);
  return buffer;
}

template <typename... Ts>
void serial_writef(const char* format_str, Ts... args) {
  char* formatted_str = format(format_str, args...);
  Serial.print(formatted_str);
}