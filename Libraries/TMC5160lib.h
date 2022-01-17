
#include "Arduino.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
extern "C"
{
	// Structs, Enums and Variables declarations
	enum stepper_mode
	{
		SPI_mode,
		UART_mode,
	};


	uint8_t initTMCSPI(uint8_t MODE = 0);
	uint8_t setSpeed(uint32_t speed);
	uint8_t setDir(uint8_t dir);
	uint8_t moveMotor(uint8_t steps);
}