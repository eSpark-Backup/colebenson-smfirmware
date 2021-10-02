#include <Arduino.h>
#include <TMCStepper.h> // v0.7.1

constexpr uint8_t cs = 53;

TMC5160Stepper driver(cs, 0.075);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
}

void loop() {
  delay(3000);
  Serial.print("Firmware version = ");
  Serial.println(driver.version(), HEX);
  Serial.print("Enabled = ");
  Serial.println(driver.drv_enn());
  Serial.print("Mode = ");
  Serial.println(driver.sd_mode() ? "Step/Dir" : "Ramp Generator");
}

void TMC2130Stepper::switchCSpin(bool state) {
  delayMicroseconds(100);
  digitalWrite(_pinCS, state);
  delayMicroseconds(100);
}
