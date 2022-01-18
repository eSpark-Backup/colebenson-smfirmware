#include "TMC5160lib.h"



// Varaibles and structs definitions
SPIClass *mySPI;
uint32_t spi_speed;
VMAX_t vmax_register;

extern "C"
{
	/*********************************************************************************
	 *	@purpose: Initializes TMC Stepper driver based on mode selected i.e. SPI or UART
	 *	
	 *	@mode: SPI_mode or UART_mode
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	uint8_t initTMC(uint8_t mode)
	{
		if(mode == SPI_MODE)
		{
			beginSPI();
		}
		
		else if(mode == UART_MODE)
		{
			beginUART();
		}
			
		return FAILED;
	}



	/**********************************************************************************
	 *	@purpose: Set stepper motor speed
	 *	
	 *	@speed: speed of motor in usteps/t; where t=2^24/fclk = ~1.4sec; range = 0...(2^23)-512
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t setSpeed(uint32_t speed)
	{
			
		return FAILED;
	}
	
	
	/***********************************************************************************
	 *	@purpose: invert stepper motor direction
	 *	
	 *	@dir: INVERT or NOT_INVERT
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t setDir(uint8_t dir)
	{
			
		return FAILED;
	}
	
	/***********************************************************************************
	 *	@purpose: move motor specified usteps 
	 *	
	 *	@steps: usteps to move; range = -2^31...(2^31)-1
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t moveMotor(int32_t steps)
	{
			
		return FAILED;
	}
	
	/***********************************************************************************
	 *	@purpose: set TMC registers
	 *	
	 *	@address: register address
	 *  @value: value to store in register address
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	static uint8_t setReg(uint8_t address, uint8_t value)
	{
		return FAILED;
	}
	
    /***********************************************************************************
	 *	@purpose: Begin SPI
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	static uint8_t beginSPI()
	{
		pinMode(CS_PIN, OUTPUT);
		digitalWrite(CS_PIN, HIGH);
		pinMode(EN_PIN, OUTPUT);
		digitalWrite(EN_PIN, LOW);
		
		mySPI =  new SPIClass(MOSI_PIN, MISO_PIN, SCK_PIN, CS_PIN);
		spi_speed = 16000000/8;
		//SPISettings(spi_speed, MSBFIRST, SPI_mode3);
		//SPI.begin();
		
		return FAILED;
	}
	
	/***********************************************************************************
	 *	@purpose: Begin UART
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	static uint8_t beginUART()
	{
		return FAILED;
	}

}