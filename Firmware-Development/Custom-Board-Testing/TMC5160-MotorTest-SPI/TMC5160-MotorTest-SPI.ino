#include <stdio.h>
#include <Arduino.h>
#include <TMCStepper.h>
#include "HardwareSerial.h"

#define EN_PIN           PC11 // Enable
#define DIR_PIN          PC8  // Direction
#define STEP_PIN         PC7  // Step
#define CS_PIN           PA4
#define MOSI_PIN         PA7
#define MISO_PIN         PA6
#define SCK_PIN          PA5
#define R_SENSE          0.0165f

//HardwareSerial hserial2(PA1, PA0); // (RX, TX)

#define hserial2 Serial
TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE, MOSI_PIN, MISO_PIN, SCK_PIN);

bool dir = true;
bool shaft = true;


void setup() { //********************************************************************************************
  hserial2.begin(115200);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, LOW);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  driver.begin();

  // Ramp Mode ON
  pinMode(PB4, OUTPUT);
  digitalWrite(PB4, LOW);
  
  //pinMode(CS_PIN, OUTPUT);
  //digitalWrite(CS_PIN, HIGH);


  driver.rms_current(1000);
  driver.en_pwm_mode(1);      // Enable extremely quiet stepping
  driver.pwm_autoscale(1);
  driver.microsteps(128);
  driver.toff(5);
  driver.irun(17);
  driver.ihold(10);
  driver.iholddelay(6);
  driver.GLOBAL_SCALER(130);
  //ramp settings
  driver.RAMPMODE(1);
  driver.VSTART(0);
  driver.a1(5);
  driver.v1(40);
  driver.AMAX(1000);
  driver.VMAX(100000);
  driver.DMAX(300);
  driver.d1(40);
  driver.VSTOP(0);
  driver.shaft(true);

  //current motor position
  driver.XACTUAL(0);
  hserial2.print("DRV_STATUS=0b");
  hserial2.println(driver.DRV_STATUS(), BIN);

}

//bool shaft = false;

void loop() {  //***************************************************************************************************
  //  delay(300);
  hserial2.print("Firmware version = ");
  hserial2.println(driver.version(), HEX);
  hserial2.print("Enabled = ");
  hserial2.println(driver.drv_enn());
  hserial2.print("Mode = ");
  hserial2.println(driver.sd_mode() ? "Step/Dir" : "Ramp Generator");
  driver.XTARGET(driver.XACTUAL() + 300000);

  /*for (uint16_t i = 5000; i > 0; i--) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(160);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(160);
  }*/

  delay(1000);
  
  /*shaft = !shaft;
  driver.shaft(shaft);*/
}

void TMC2130Stepper::switchCSpin(bool state) {
  delayMicroseconds(100);
  digitalWrite(_pinCS, state);
  delayMicroseconds(100);
}
