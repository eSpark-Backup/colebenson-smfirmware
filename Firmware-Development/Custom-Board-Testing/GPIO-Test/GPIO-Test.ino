const int pin = PA13;

void setup() {
  pinMode(pin, OUTPUT);

}

void loop() {
  digitalWrite(pin, LOW);
  delay(1000);
  digitalWrite(pin, HIGH);
  delay(1000);
}
