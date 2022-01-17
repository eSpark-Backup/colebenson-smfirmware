#include "SPI.h"

#define CS_PIN PA4
#define MOSI_PIN PA7
#define MISO_PIN PA6
#define SCK_PIN PA5

SPIClass mySPI(MOSI_PIN, MISO_PIN, SCK_PIN);
void setup() {
  // put your setup code here, to run once:
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);

  SPI.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Response");
  Serial.println(SPI.transfer(0x33));

}
