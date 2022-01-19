#include "TMC5160lib.h"

void setup() {
  // put your setup code here, to run once:
  initTMC(SPI_MODE);
  setSpeed(100000);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveMotor(2000);

  delay(1000);

  setDir(INVERT);
  moveMotor(2000);

  delay(1000);

  setDir(INVERT);
}
