const int pin = 61;

void setup() {
  pinMode (pin, OUTPUT);
  delay(100);
  digitalWrite(pin, HIGH);

}

void loop() {
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);

}
