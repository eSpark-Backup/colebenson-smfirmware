#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"

HardwareSerial hserial4(1, 0);
HardwareSerial hserial2(3, 2);

void setup() {
  
  hserial4.begin(115200);
  hserial2.begin(115200);

}

void loop() {
  while (hserial4.available() > 0)
  {
    hserial2.write(hserial4.read());
  }

  while (hserial2.available() > 0)
  {
    hserial4.write(hserial2.read());
  }

}
