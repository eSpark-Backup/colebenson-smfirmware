#include "extern_eeprom.h"

uint8_t value_store = 29;
uint8_t value_retrieve = 0;
uint16_t address = 0;

void setup() {
  // put your setup code here, to run once:

  initEEPROM();
  

}

void loop() {
  // put your main code here, to run repeatedly:

  retrieveByte(address, &value_retrieve);
  Serial.print("Before update, address: "); Serial.println(address);
  Serial.println(value_retrieve);

  delay(100);
  
  storeByte(address, value_store);

  delay(100);

  retrieveByte(address, &value_retrieve);
  
  Serial.print("After update, address: "); Serial.println(address);
  Serial.println(value_retrieve);

  delay(1000);

  address++;

}
