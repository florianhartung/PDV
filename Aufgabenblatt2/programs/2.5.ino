/**
 * Aufgabe 2.5
 * 
 * Erstellen Sie ein Programm für das STTS-Arduino-System,
 * das die Position eines Tasters und eine DIP-Schalters
 * über die serielle Verbindung ausgibt.
 */

#include "util.h"
#include "digital_pin_util.h"

const char* on_message = 
"  Pin %d \n"
"    on   \n"
"---------\n"
"|   ##   |\n"
"|   ##   |\n"
"|        |\n"
"----------\n"
"    off  \n";

const char* off_message = 
"  Pin %d \n"
"    on   \n"
"---------\n"
"|        |\n"
"|   ##   |\n"
"|   ##   |\n"
"----------\n"
"    off  \n";

void setup5() {
  Serial.begin(9600);
  //pinMode(9, INPUT);
  pins_set(0, 30, INPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
}

void loop5() {
  for(int pin = 0; pin < digital_pin_count; pin++) {
    Serial.end();
    pinMode(0, INPUT);
    pinMode(1, INPUT);
    auto pin_status = dip_read(pin);
    Serial.begin(9600);
    serial_writef("%d ", pin_status);
  }
  Serial.print("\n");
  delay(500);  
}