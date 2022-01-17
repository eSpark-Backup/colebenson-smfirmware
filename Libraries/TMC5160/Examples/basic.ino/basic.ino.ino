#include "TMC5160lib.h"

void setup() {
  // put your setup code here, to run once:
  initTMCSPI(SPI_mode);
  Serial.println(FAILED + 5);
}

void loop() {
  // put your main code here, to run repeatedly:

}
