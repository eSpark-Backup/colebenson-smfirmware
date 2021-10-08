#include <SoftwareSerial.h>

const int led  = 60;
const int led2 = 61;

SoftwareSerial mySerial(3, 2); // RX, TX

void setup() {
  mySerial.begin(115200);
  pinMode(led,  OUTPUT);
  pinMode(led2, OUTPUT);
  delay (100);
}

void loop() {
  mySerial.println("Test");
  delay(500);
  digitalWrite(led,  HIGH);
  digitalWrite(led2, HIGH);
  delay(500);
  digitalWrite(led,  LOW);
  digitalWrite(led2, LOW);
  delay(500);
}
