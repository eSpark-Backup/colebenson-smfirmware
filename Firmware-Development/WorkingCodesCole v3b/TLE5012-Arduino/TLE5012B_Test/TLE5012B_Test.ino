#include <Arduino.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/tle5012b/MagneticSensorTLE5012B.h"

double TLE5012GetAngle();

#define DELETE_BIT_15               0x7FFF    //!< \brief Value used to delete everything except the first 15 bits
#define CHANGE_UINT_TO_INT_15       0x8000    //!< \brief Value used to change unsigned 16bit integer into signed
#define CHECK_BIT_14                0x4000    //!< \brief
#define POW_2_15                    32768.0   //!< \brief values used to for final calculations of angle speed, revolutions, range and value
#define ANGLE_360_VAL               360.0     //!< \brief

#define ENC_DATA  PB15
#define ENC_SCK   PB13
#define ENC_CS    PB12
MagneticSensorTLE5012B tle5012b(ENC_DATA, ENC_SCK, ENC_CS);

// #define UART_RX PC7
// #define UART_TX PC6

void setup() 
{
  // Serial.setRx(UART_RX);
  // Serial.setTx(UART_TX);
  Serial.begin(115200);
  // while(!Serial);
  
  tle5012b.init();
}

void loop() 
{
  double angle = TLE5012GetAngle();
  delay(1000);
  Serial.println(angle);
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