#pragma once

#include "Arduino.h"
#include "SPI.h"

void initTMCSPI();
void setSpeed(unsigned long speed);
void setDir(uint8_t dir);
void moveMotor(uint8_t steps);