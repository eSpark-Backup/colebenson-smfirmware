#include <stdio.h>
#include <Arduino.h>
#include <TMCStepper.h>
#include "HardwareSerial.h"

#define EN_PIN           PC2 // Enable
#define DIR_PIN          PC0  // Direction
#define STEP_PIN         PC1  // Step
#define CS_PIN           PA4
#define MOSI_PIN         PA7
#define MISO_PIN         PA6
#define SCK_PIN          PA5
#define R_SENSE          0.075f

HardwareSerial hserial2(PA1, PA0); // (RX, TX)
TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE, MOSI_PIN, MISO_PIN, SCK_PIN);

bool dir = true;

void setup() {
  hserial2.begin(115200);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  
  driver.begin();
  
  //pinMode(CS_PIN, OUTPUT);
  //digitalWrite(CS_PIN, HIGH);


  driver.rms_current(600); 
  driver.en_pwm_mode(1);      // Enable extremely quiet stepping
  driver.pwm_autoscale(1);
  driver.microsteps(128);
  driver.toff(1);
  driver.irun(17);
  driver.ihold(10);
  driver.iholddelay(6);
  driver.GLOBAL_SCALER(130);
  //ramp settings
  driver.RAMPMODE(0);
  driver.VSTART(0);
  driver.a1(5);
  driver.v1(40);
  driver.AMAX(150);
  driver.VMAX(10000);
  driver.DMAX(300);
  driver.d1(40);
  driver.VSTOP(0);

  //current motor position
  driver.XACTUAL(0);
  hserial2.print("DRV_STATUS=0b");
  hserial2.println(driver.DRV_STATUS(), BIN);
  
}

void loop() {
//  delay(300);
  hserial2.print("Firmware version = ");
  hserial2.println(driver.version(), HEX);
  hserial2.print("Enabled = ");
  hserial2.println(driver.drv_enn());
  hserial2.print("Mode = ");
  hserial2.println(driver.sd_mode() ? "Step/Dir" : "Ramp Generator");
 delay(1000);
 driver.XTARGET(driver.XACTUAL() + 300000);
} 

void TMC2130Stepper::switchCSpin(bool state) {
  delayMicroseconds(100);
  digitalWrite(_pinCS, state);
  delayMicroseconds(100);
}
