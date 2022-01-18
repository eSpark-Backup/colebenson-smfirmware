#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "TMCbitfields.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
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
	extern SPISettings *settings;
	extern DRVCONF_t DRVCONF_register;


	// Public functions
	uint8_t initTMC(uint8_t mode = SPI_MODE);
	uint8_t setSpeed(uint32_t speed);
	uint8_t setDir(uint8_t dir = NOT_INVERT);
	uint8_t moveMotor(int32_t steps);
	
	// Private functions
	static uint8_t beginSPI();
	static uint8_t beginUART();
	static uint8_t rms_current(uint16_t mA);
	static uint8_t write(uint8_t addressByte, uint32_t config);
	static void vsense(uint8_t B);
	static void irun(uint8_t B);
	static void ihold(uint8_t B);
	static void en_pwm_mode(uint8_t B);
	static void pwm_autoscale(uint8_t B);
	static void microsteps(uint16_t ms);
	static void toff(uint8_t B);
	static void irun(uint8_t B);
	static void ihold(uint8_t B);
	static void iholddelay(uint8_t B);
	static void GLOBAL_SCALER(uint8_t input);
	static void RAMPMODE(uint8_t input);
	static void VSTART(uint32_t input);
	static void a1(uint16_t input);
	static void v1(uint32_t input);
	static void AMAX(uint16_t input) ;
	static void VMAX(uint32_t input);
	static void DMAX(uint16_t input);
	static void d1(uint16_t input) ;
	static void VSTOP(uint32_t input) ;
	static void shaft(uint8_t B);
	static void XACTUAL(int32_t input);
	
}