#include "extern_eeprom.h"

uint8_t value_store = 25;
uint8_t value_retrieve;
uint16_t address = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  initEEPROM();

}

void loop() {
  // put your main code here, to run repeatedly:

  retrieveByte(address, &value_retrieve);
  Serial.print("Before update, address: "); Serial.println(address);
  Serial.println(value_retrieve);


  storeByte(address, value_store);

  delay(100);

  retrieveByte(address, &value_retrieve);
  Serial.print("After update, address: "); Serial.println(address);
  Serial.println(value_retrieve);

  delay(1000);

  address++;

}
