#pragma once

#define array_size(array) (sizeof(array) / sizeof(*(array)))

/**
 * Mehrfaches Reservieren von Speicher hat wahrscheinlich zu Speicherfragmentierung geführt, was zu
 * Abstürzen des Programms geführt hat. (Obwohl die buffer wieder befreit wurden in serial_writef)
 * Deshalb ist diese Lösung nicht sehr schön.
 */
template <typename... Ts>
char* format(const char* format_str, Ts... args) {
  // Statischer buffer, sodass dieser immer wieder verwendet werden kann
  static char* buffer;
  static bool buffer_has_been_initialized;

  //Berechnet die Größe die für den voll fomattierten String benötigt wird
  int needed = snprintf(nullptr, 0, format_str, args...) + 1;

  if(!buffer_has_been_initialized) {
    // Wenn buffer noch nie allocated wurde, kann realloc nicht aufgerufen werden, deswegen normales calloc
    buffer = calloc(needed, sizeof(char));
    buffer_has_been_initialized = true;
  } else {
    buffer = realloc(buffer, needed);
  }
  
  sprintf(buffer, format_str, args...);
  return buffer;
}

/**
 * Überträgt einen formattierten String über die serielle Schnittstelle
 */
template <typename... Ts>
void serial_writef(const char* format_str, Ts... args) {
  char* formatted_str = format(format_str, args...);
  Serial.print(formatted_str);
}
