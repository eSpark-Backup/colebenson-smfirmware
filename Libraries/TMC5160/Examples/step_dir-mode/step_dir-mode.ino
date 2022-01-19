#include "TMC5160lib.h"

void setup() {
  // put your setup code here, to run once:
  initTMC(STEP_MODE);
  setSpeed(100000);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveMotor(2000);

  delay(1000);

  setDir(BACKWARD);
  moveMotor(2000);

  delay(1000);

  setDir(FORWARD);
}
