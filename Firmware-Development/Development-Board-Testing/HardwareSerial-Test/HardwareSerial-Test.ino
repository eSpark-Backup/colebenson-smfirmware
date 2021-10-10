#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"

//HardwareSerial hserial4(1, 0);
char a[2];
HardwareSerial hserial2(PA3, PA2); // (RX, TX)

void setup() {
  
  //hserial4.begin(115200);
  hserial2.begin(115200);

}

void loop() {

/* Dual Serial Test Code */
//  while (hserial4.available() > 0)
//  {
//    hserial2.write(hserial4.read());
//  }
//
//  while (hserial2.available() > 0)
//  {
//    hserial4.write(hserial2.read());
//  }

/* Single Serial Test Code */
  if (hserial2.available() > 0) {
    hserial2.readBytes(a, 2);
    
    if(a[0] == '$' && a[1] == '#')
      hserial2.println("$#");
  }
}
