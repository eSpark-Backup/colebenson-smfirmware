#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "TMCbitfields.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define EN_PIN           PC11
#define CS_PIN           PA4
#define MOSI_PIN         PA7
#define MISO_PIN         PA6
#define SCK_PIN          PA5
#define R_SENSE          0.0165f


extern "C"
{
	// Structs, Enums and Variables declarations
	enum stepper_mode
	{
		SPI_MODE,
		UART_MODE,
	};

	enum stepper_dir
	{
		NOT_INVERT,
		INVERT,
	};
	
	extern SPIClass *mySPI;
	extern uint32_t spi_speed;
	extern VMAX_t vmax_register;


	// Public functions
	uint8_t initTMC(uint8_t mode = SPI_MODE);
	uint8_t setSpeed(uint32_t speed);
	uint8_t setDir(uint8_t dir = NOT_INVERT);
	uint8_t moveMotor(int32_t steps);
	
	// Private functions
	static uint8_t setReg(uint8_t address, uint8_t value);
	static uint8_t beginSPI();
	static uint8_t beginUART();
}