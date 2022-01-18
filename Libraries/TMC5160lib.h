
#include "Arduino.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1

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

	uint8_t initTMC(uint8_t mode = SPI_MODE);
	uint8_t setSpeed(uint32_t speed);
	uint8_t setDir(uint8_t dir);
	uint8_t moveMotor(int32_t steps);
}