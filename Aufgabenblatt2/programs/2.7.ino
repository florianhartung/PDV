/**
 * Aufgabe 2.7
 * 
 * Erstellen Sie ein Programm für das STTS-Arduino-System,
 * das die 8 LEDs, die mit dem rechte DIP Schalter verbunden,
 * sind in einer Lauflichterkette nacheinander ansteuert.
 * 
 * DIP Schalter 1-7 legt die Anzahl der wandernden LED fest 
 *
 * DIP Schalter 1-8 legt die Art der Lichterkette fest (Invertierung)
 *
 * Wenn ein Taster (hier Taster an Pin 13) gedrückt gehalten wird, ändert sich die Laufrichtung.
 */

#include "digital_pin_util.h"
#define LED_START_INDEX 8
#define DELAY_BETWEEN_UPDATES 69
#define DIRECTION_SWITCH_BUTTON_PIN 13

uint8_t led_state = 0b10000000;

unsigned long last_update_millis = 0;

int last_dip7 = LOW;

void setup7() {
  pins_set(6, 7, INPUT);
  pinMode(DIRECTION_SWITCH_BUTTON_PIN, INPUT);
  pins_set(LED_START_INDEX, LED_START_INDEX + 7, OUTPUT);
}

void loop7() {
  unsigned long current_millis = millis();

  if (current_millis - last_update_millis >= DELAY_BETWEEN_UPDATES) {
    last_update_millis = current_millis;    
  
    // Bits von led_state nach links/rechts rotieren
    if (digitalRead(DIRECTION_SWITCH_BUTTON_PIN)) {
      led_state = (led_state << 1) | (led_state >> 7);
    } else {
      led_state = (led_state >> 1) | (led_state << 7);
    }

    // DIP1-81(index = 7), welcher zuständig für das Invertieren der LEDs ist, einlesen
    bool invert_leds = dip_read(7);

    // Bits aus led_state auslesen und LEDs setzen
    for (int i = 0; i < 8; i++) {
      int current_led_state = led_state & (1 << (7-i));
      if (invert_leds) {
        current_led_state = current_led_state ? LOW : HIGH;
      }
      led_set(LED_START_INDEX + i, current_led_state);
    }
  }

  // Erkennen ob sich DIP1-7(mit index = 6), welcher zuständig für die Anzahl der laufenden LEDs ist, geändert hat
  int dip7 = dip_read(6);
  if (dip7 != last_dip7) {
    last_dip7 = dip7;

    // Neuen Status von DIP1-7 auswerten
    if (dip7 == HIGH) {
      led_state = 0b11000000;
    } else {
      led_state = 0b10000000;
    }
  }
}