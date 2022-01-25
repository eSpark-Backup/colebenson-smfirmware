#include "CAN.h"

uint16_t word_tx = 1000;
uint16_t word_rx;
uint8_t channel = 1;
uint8_t init_flag;

void setup() {
  // put your setup code here, to run once:
  initCAN();

  init_flag = initCAN();

  Serial.println("Init");
  Serial.println(init_flag);

  enableTerminal();

  /*pinMode(PC1, OUTPUT);
  pinMode(PC2, OUTPUT);

  digitalWrite(PC1, HIGH);
  digitalWrite(PC2, HIGH);*/

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Transmit Word");
  Serial.println(word_tx);

  transmitWord(word_tx, 0x407, channel);

  delay(100);

  uint8_t rcv = receiveWord(&word_rx, channel);

  Serial.println("Word received");
  Serial.println(rcv);
  Serial.println(word_rx);

  delay(1000);

}
