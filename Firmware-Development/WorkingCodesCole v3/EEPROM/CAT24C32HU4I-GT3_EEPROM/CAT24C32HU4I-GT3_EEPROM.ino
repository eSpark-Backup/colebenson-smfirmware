#include "Wire.h"

#define I2C_addr 0x50

TwoWire I2C(PC9, PA8);

#define Wire I2C

uint8_t mem_addr = 0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  //Wire.setClock(400000);

  pinMode(PA10, OUTPUT);
  digitalWrite(PA10, LOW);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Memory address");
  Serial.println(mem_addr);

  /*
     Read selective address at byte
  */
  Wire.beginTransmission(I2C_addr);

  Wire.write((mem_addr) >> 8);
  Wire.write((mem_addr) & (0xFF));

  Serial.println("Response 1");
  Serial.println(Wire.endTransmission(false));

  Wire.requestFrom(I2C_addr, 1);

  while (Wire.available())
  {
    Serial.println("Byte before update");
    Serial.println(Wire.read());
  }


  /*
     Write to mem_addr
  */
  Wire.beginTransmission(I2C_addr);

  Wire.write((mem_addr) >> 8);
  Wire.write((mem_addr) & (0xFF));

  Wire.write(20);

  Serial.println("Response 2");
  Serial.println(Wire.endTransmission(true));

  delay(10);


  /*
     Read address at byte
  */
  Wire.beginTransmission(I2C_addr);

  Wire.write((mem_addr) >> 8);
  Wire.write((mem_addr) & (0xFF));

  Serial.println("Response 3");
  Serial.println(Wire.endTransmission(false));

  Wire.requestFrom(I2C_addr, 1);

  while (Wire.available())
  {
    Serial.println("Byte after update");
    Serial.println(Wire.read());
  }


  mem_addr++;

  delay(500);
}
