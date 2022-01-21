#pragma once

#include "Arduino.h"
#include "Wire.h"

// Constants and macros
#define ADDRESS_EEPROM 0x50
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define SDA_EEPROM PC9
#define SCK_EEPROM PA8 
#define WP		   PA10

extern "C"
{
	//Public functions
	uint8_t initEEPROM(uint32_t SDA = SDA_EEPROM, uint32_t SCL = SCK_EEPROM);
	uint8_t storeByte(uint16_t address, uint8_t value);
	uint8_t retrieveByte(uint16_t address, uint8_t* value);
	
}