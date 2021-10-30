#include <SPI.h>

SPIClass mySPI(PB15, PB14, PB13);

void setup() {
  Serial.begin(115200);
  pinMode(PB12, OUTPUT);
  //mySPI.setDataMode(SPI_MODE3);
  //mySPI.setClockDivider(SPI_CLOCK_DIV128);
  //mySPI.setBitOrder(MSBFIRST);
  mySPI.beginTransaction(SPISettings(SPI_CLOCK_DIV128, MSBFIRST, SPI_MODE3));

}

void loop() {
  digitalWrite(PB12, HIGH);
//  mySPI.transfer(0x84);
  Serial.println(mySPI.transfer(0x00), HEX);
  digitalWrite(PB12, LOW);

//  digitalWrite(PB12, LOW);
//  Serial.print("Data[LOW]");
//  Serial.println(mySPI.transfer(0x00), HEX);
//  delay(300);
  

}
