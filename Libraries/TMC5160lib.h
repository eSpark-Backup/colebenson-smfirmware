#pragma once

#include "Arduino.h"
#include "SPI.h"


// Structs, Enums and Variables declarations
enum stepper_mode
{
	SPI = 1,
	UART = 2,
};

uint8_t initTMCSPI(uint8_t MODE = SPI);
uint8_t setSpeed(uint32_t speed);
uint8_t setDir(uint8_t dir);
uint8_t moveMotor(uint8_t steps);