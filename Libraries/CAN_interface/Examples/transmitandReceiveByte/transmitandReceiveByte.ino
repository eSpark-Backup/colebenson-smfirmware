#include "CAN.h"

uint8_t byte_tx = 220;
uint8_t byte_rx;
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
  Serial.println("Transmit Byte");
  Serial.println(byte_tx);

  transmitByte(byte_tx, 0x407, channel);

  delay(100);

  uint8_t rcv = receiveByte(&byte_rx, channel);

  Serial.println("Byte received");
  Serial.println(rcv);
  Serial.println(byte_rx);

  delay(1000);

}
