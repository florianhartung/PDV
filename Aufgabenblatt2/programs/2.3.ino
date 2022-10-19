/**
 * Aufgabe 2.3
 *
 * Erstellen Sie mit der Arduino IDE ein erstes Hello-World Arduino Projekt,
 * das "Hello STTS Arduino World" über den Seriellen Port ausgibt. 
 */

void setup3() {
  Serial.begin(9600);
}

void loop3() {
  Serial.print("Hello STTS Arduino World\n");
  delay(100);
}
