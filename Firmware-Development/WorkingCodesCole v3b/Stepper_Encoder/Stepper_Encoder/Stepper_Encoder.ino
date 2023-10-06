#include <Arduino.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/tle5012b/MagneticSensorTLE5012B.h"
#include <Arduino.h>
#include <TMCStepper.h>

#define EN_PIN           PC11 // Enable
#define DIR_PIN          PC8  // Direction
#define STEP_PIN         PC7  // Step
#define CS_PIN           PA4 // Chip select
#define SW_MOSI          PA7 // Software Master Out Slave In (MOSI)
#define SW_MISO          PA6 // Software Master In Slave Out (MISO)
#define SW_SCK           PA5 // Software Slave Clock (SCK)
#define SW_RX            PA1 // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX            PA0 // TMC2208/TMC2224 SoftwareSerial transmit pin
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.0165f // Match to your driver

TMC5160Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);

#define DELETE_BIT_15               0x7FFF  
#define CHANGE_UINT_TO_INT_15       0x8000  
#define CHECK_BIT_14                0x4000  
#define POW_2_15                    32768.0 
#define ANGLE_360_VAL               360.0   

#define ENC_DATA  PB15
#define ENC_SCK   PB13
#define ENC_CS    PB12
MagneticSensorTLE5012B tle5012b(ENC_DATA, ENC_SCK, ENC_CS);

HardwareTimer *timer = new HardwareTimer(TIM3);
bool ISRflag=false;
bool shaft = false;
double zeroRef=0; 

void onTimer() {
  ISRflag=true;
}

double TLE5012GetAngle();

void setup() {
  Serial.begin(115200);
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);      // Enable driver in hardware
  timer->setPrescaleFactor(8399); 
  timer->setOverflow(10000);      
  timer->attachInterrupt(onTimer);
  timer->resume();
  tle5012b.init();
  zeroRef = TLE5012GetAngle();
  Serial.print("The zero reference is: ");
  Serial.println(zeroRef);

  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  driver.toff(5);                 // Enables driver in software
  driver.rms_current(600);        // Set motor RMS current
  driver.microsteps(16);          // Set microsteps to 1/16th

  driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.pwm_autoscale(true);     // Needed for stealthChop
  tone(STEP_PIN, 1000);
}

void loop() {
    if(ISRflag)
    {
    double angle = TLE5012GetAngle();
      if(angle<zeroRef)
      {
        angle=(angle-zeroRef)+360;
      }
      else
      {
      angle-=zeroRef;
      }
    Serial.println(angle);
    ISRflag=false;
    }
}

double TLE5012GetAngle()
{
  tle5012b.update();
  uint16_t rawData = tle5012b.readRawAngle();

  rawData = (rawData & (DELETE_BIT_15));

  //check if the value received is positive or negative
  if (rawData & CHECK_BIT_14) 
  {
    rawData = rawData - CHANGE_UINT_TO_INT_15;
  }
  int16_t rawAnglevalue = rawData;
  double angleValue = (ANGLE_360_VAL / POW_2_15) * ((double)rawAnglevalue);
  if (angleValue<0)
  {
    angleValue = 360 + angleValue;
  }
  
  return angleValue;
}
