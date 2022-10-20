/**
 * Aufgabe 2.5
 * 
 * Erstellen Sie ein Programm für das STTS-Arduino-System,
 * das die Position eines Tasters und eine DIP-Schalters
 * über die serielle Verbindung ausgibt.
 */

#include "util.h"
#include "digital_pin_util.h"

constexpr int serial_baud_rate = 19200; 

constexpr int dip_start = 0;
constexpr int dip_end = digital_pin_count - 1;

constexpr const static inline char* pin_layout_format = "D %d-%d | ";
constexpr const char* button_layout_format = "Btn %d | ";
constexpr const char* status_format = "%5s | ";

//Schreibt den String D n-n | D n-n+1 | ...
void write_dip_layout_string(int pin_start, int pin_end) {
  for(int pin = pin_start; pin <= pin_end; pin++) {
    serial_writef(pin_layout_format, pin / 8 + 1, pin % 8 + 1);
  }
}

//Schreibt den String Btn n | Btn n + 1 | ...
void write_button_layout_string(int pin_start, int pin_end) {
  for(int pin = pin_start; pin <= pin_end; pin++) {
    serial_writef(button_layout_format, pin + 1 - pin_start);
  }
}

void setup5() {
  Serial.begin(serial_baud_rate);

  // DIP Pins auf Input setzen
  pins_set(dip_start, dip_end, INPUT);

  // Taster auf Input setzen
  for(int i = 10; i < 13; i++) {
    pinMode(i, INPUT);
  }
}

void loop5() {
  // Monitorinhalte nach oben verschieben damit aktueller Inhalt im Fokus steht
  for(int i = 0; i < 10; i++) {
    Serial.println();
  }

  // DIP Schalter und Taster Tabellenkop
  write_dip_layout_string(dip_start, dip_end);
  write_button_layout_string(10, 13);
  
  Serial.println();

  // DIP Pins lesen
  for(int pin = dip_start; pin <= dip_end; pin++) {
    Serial.end();
    int pin_status = dip_read(pin);
    Serial.begin(serial_baud_rate);
    serial_writef(status_format, (pin_status ? "on" : ""));
  }

  // Taster lesen
  for(int button = 0; button < 4; button++) {
    int button_status = button_read(button);
    serial_writef(status_format, button_status ? "on" : "");
  }

  Serial.println("\n");

  delay(500);  
}
