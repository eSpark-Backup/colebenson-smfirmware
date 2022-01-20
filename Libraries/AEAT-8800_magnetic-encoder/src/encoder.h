#pragma once

#include "Arduino.h"
#include "SPI.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define EN_MOSI PB15
#define EN_MISO PB14
#define EN_SCK  PB13
#define EN_CS	PB12

extern "C"
{
	
	// Public functions
	uint8_t initEncoder();
	uint8_t getAngle(uint16_t *dataOut);
	
	//Private functions
	static void applicationInit();
	static void angle4_startMesuremenet();
	static uint8_t angle4_getNewAngle(uint16_t *dataOut);
}