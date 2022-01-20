#include "extern_eeprom.h"

uint8_t value_store = 25;
uint8_t value_retrieve;
uint16_t address = 100;

void setup() {
  // put your setup code here, to run once:
  initEEPROM();

}

void loop() {
  // put your main code here, to run repeatedly:

  storeByte(address, value_store);

  delay(100);

  retrieveByte(address, &value_retrieve);

  delay(1000);

}
