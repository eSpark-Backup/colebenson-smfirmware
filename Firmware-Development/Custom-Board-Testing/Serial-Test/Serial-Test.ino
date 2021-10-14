void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("Test: ");
  Serial.println(F_CPU);
  delay(1000); 
}
