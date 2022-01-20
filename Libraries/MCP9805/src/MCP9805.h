#pragma once

#include "Arduino.h"
#include "Wire.h"

// Constants and macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define SDA_MCP PB11
#define SCK_MCP PB10
#define ADDRESS_MCP 0x10

extern "C"
{
	//Public functions
	uint8_t initMCP();
	uint8_t getTempC(float *tempC);
	uint8_t getTempF(float *tempF);
}