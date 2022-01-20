#pragma once

#include "Arduino.h"
#include "SPI.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
   

extern "C"
{
	
	// Public functions
	uint8_t initEncoder();
	uint8_t getAngle(uint16_t *dataOut);
	
	//Private functions
}