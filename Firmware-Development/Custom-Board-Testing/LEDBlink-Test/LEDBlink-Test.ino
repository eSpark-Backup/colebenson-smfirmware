#define LED_B  PD10
#define LED_R  PD12
#define LED_G  PD13

void setup() {
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
}

void loop() {
  digitalWrite(LED_B, HIGH);
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  delay(500);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  delay(500); 
}
