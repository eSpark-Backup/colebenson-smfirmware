#include "CAN.h"

uint8_t byte_tx = 200;
uint8_t byte_rx;
uint8_t channel = 2;

void setup() {
  // put your setup code here, to run once:
  initCAN();

}

void loop() {
  // put your main code here, to run repeatedly:
  transmitByte(byte_tx, 0x407, channel);

  receiveByte(&byte_rx, channel);

  delay(1000);
  
}
