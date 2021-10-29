#include <SPI.h>

SPIClass mySPI(PB15, PB14, PB13);

void setup() {
  Serial.begin(9600);
  pinMode(PB12, OUTPUT);
  mySPI.setDataMode(SPI_MODE3);
  mySPI.setClockDivider(SPI_CLOCK_DIV4);
  mySPI.begin();
  
 /* uint8_t x[2];
  x[0] = 0x06;
  x[1] = 0x04;

  mySPI.transfer(x, 2); */

}

void loop() {
  uint8_t readReg[ 3 ] = { 0 };
  uint16_t angleData;
  uint8_t status;

  digitalWrite(PB12, HIGH);
  mySPI.transfer(readReg, 3);
  digitalWrite(PB12, LOW);

  angleData = readReg[0];
  angleData = angleData << 8;
  angleData = angleData | readReg[0];

  status = readReg[2] >> 4;

  Serial.print("Angle: ");
  Serial.println(angleData);

  Serial.print("Status: ");
  Serial.println(status);

  delay(100);
}
