#include "encoder.h"

uint16_t angle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initEncoder();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t status = getAngle(&angle);

  Serial.println("Get Angle");
  Serial.println(angle);

  Serial.println("Get Status");
  Serial.println(status);

  delay(1000);

}
