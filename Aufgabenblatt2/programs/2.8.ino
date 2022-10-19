/**
 * Aufgabe 2.8
 *
 * Entwickeln Sie ein Programm, das abhängig vom Binär-Wort
 * der DIP Schaltern die Helligkeit der LED ändert!
 *
 */

#define LED_PWM_PIN 3
#define DIP_SWITCH_START_INDEX 8

void setup8() {
  pins_set(DIP_SWITCH_START_INDEX, DIP_SWITCH_START_INDEX + 7, INPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
}

void loop8() {
  // dip_read_word gibt wert von 0 bis 255 für die dip group `lower` zurück
  int dip_value = dip_read_word(lower);

  analogWrite(LED_PWM_PIN, dip_value);
  
  // Verzögerung, da sonst PWM nicht funktioniert
  delay(50);
}
