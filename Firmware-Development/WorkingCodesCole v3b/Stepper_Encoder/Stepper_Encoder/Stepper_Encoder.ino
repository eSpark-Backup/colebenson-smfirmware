#include <Arduino.h>
#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>
#include "encoders/tle5012b/MagneticSensorTLE5012B.h"
#include <SPI.h>

int chipCS = PA4;
const byte STEPOUT = PC7;
int enable = PC11;

#define EN_PIN           PC11 // Enable
#define DIR_PIN          PC8  // Direction
#define STEP_PIN         PC7  // Step
#define CS_PIN           PA4 // Chip select
#define SW_MOSI          PA7 // Software Master Out Slave In (MOSI)
#define SW_MISO          PA6 // Software Master In Slave Out (MISO)
#define SW_SCK           PA5 // Software Slave Clock (SCK)
#define SW_RX            PA1 // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX            PA0 // TMC2208/TMC2224 SoftwareSerial transmit pin
SPIClass mySPI(SW_MOSI, SW_MISO, SW_SCK);

#define SPI mySPI

// Stepper interval time
const long interval = 160;
unsigned long previousMillis = 0;

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
  
  pinMode(chipCS, OUTPUT);
  pinMode(STEPOUT, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(chipCS, HIGH);
  digitalWrite(enable, HIGH);
  digitalWrite(STEPOUT, LOW);

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  // init motor
  // ---------------------
  SPISendData(0x80, 0x00000080);     // GCONF
  SPISendData(0xED, 0x00000000);     // SGT
  SPISendData(0x94, 0x00000040);     // TCOOLTHRS
  SPISendData(0x89, 0x00010606);     // SHORTCONF
  SPISendData(0x90, 0x00080303);     // IHOLD_IRUN
  SPISendData(0xEC, 0x15410153);     // CHOPCONF - microstepping 8
  SPISendData(0xF0, 0xC40C001E);     // PWMCONF

  // ---------------------
  digitalWrite(enable, LOW);

  Serial.begin(115200);
  timer->setPrescaleFactor(8399); 
  timer->setOverflow(10000);      
  timer->attachInterrupt(onTimer);
  timer->resume();
  tle5012b.init();
  zeroRef = TLE5012GetAngle();
}

void loop() {
  for (uint16_t i = 5000; i > 0; i--) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      digitalWrite(STEP_PIN, HIGH);
    }
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      digitalWrite(STEP_PIN, LOW);
    }
  }
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

void SPISendData(unsigned long address, unsigned long datagram)
{
  //TMC5130 takes 40 bit data: 8 address and 32 data
  digitalWrite(chipCS, LOW);
  delayMicroseconds(10);

  SPI.transfer(address);
  SPI.transfer((datagram >> 24) & 0xff);
  SPI.transfer((datagram >> 16) & 0xff);
  SPI.transfer((datagram >> 8) & 0xff);
  SPI.transfer((datagram) & 0xff);
  digitalWrite(chipCS, HIGH);
}

