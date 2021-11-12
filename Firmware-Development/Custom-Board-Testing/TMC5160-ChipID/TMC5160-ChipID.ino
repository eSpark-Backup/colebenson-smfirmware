#include <stdio.h>
#include <Arduino.h>
#include <TMCStepper.h>
#include "HardwareSerial.h"

#define CS_PIN PA4
#define MOSI_PIN PA7
#define MISO_PIN PA6
#define SCK_PIN PA5
#define R_SENSE 0.0165f

//HardwareSerial hserial2(PA1, PA0); // (RX, TX)

#define hserial2 Serial

TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE, MOSI_PIN, MISO_PIN, SCK_PIN);

void setup() {
  hserial2.begin(115200);
  driver.begin();
  
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
}

void loop() {
  delay(3000);
  hserial2.print("Firmware version = ");
  hserial2.println(driver.version(), HEX);
  hserial2.print("Enabled = ");
  hserial2.println(driver.drv_enn());
  hserial2.print("Mode = ");
  hserial2.println(driver.sd_mode() ? "Step/Dir" : "Ramp Generator");
} 

void TMC2130Stepper::switchCSpin(bool state) {
  delayMicroseconds(100);
  digitalWrite(_pinCS, state);
  delayMicroseconds(100);
}
