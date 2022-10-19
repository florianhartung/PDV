#include "util.h"
#include "digital_pin_util.h"

constexpr int serial_baud_rate = 19200; 

constexpr int dip_start = 0;
constexpr int dip_end = digital_pin_count - 1;

const char* pin_layout_format = "D %d-%d | ";
const char* button_layout_format = "Btn %d | ";
const char* status_format = "%5s | ";

void write_dip_layout_string(int pin_start, int pin_end) {
  for(int pin = pin_start; pin <= pin_end; pin++) {
    serial_writef(pin_layout_format, pin / 8 + 1, pin % 8 + 1);
  }
}

void write_button_layout_string(int pin_start, int pin_end) {
  for(int pin = pin_start; pin <= pin_end; pin++) {
    serial_writef(button_layout_format, pin + 1 - pin_start);
  }
}

void setup5() {
  Serial.begin(serial_baud_rate);
  pins_set(dip_start, dip_end, INPUT);
  for(int i = 10; i < 13; i++) {
    pinMode(i, INPUT);
  }
}

void loop5() {
  Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  
  write_dip_layout_string(dip_start, dip_end);
  write_button_layout_string(10, 13);
  
  Serial.println();

  for(int pin = dip_start; pin <= dip_end; pin++) {
    Serial.end();
    int pin_status = dip_read(pin);
    Serial.begin(serial_baud_rate);
    serial_writef(status_format, (pin_status ? "on" : ""));
  }

  for(int button = 0; button < 4; button++) {
    int button_status = button_read(button);
    serial_writef(status_format, button_status ? "on" : "");
  }
  Serial.println("\n");
  delay(500);  
}