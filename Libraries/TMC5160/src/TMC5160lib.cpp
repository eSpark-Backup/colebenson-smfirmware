#include "TMC5160lib.h"
#include "Arduino.h"
#include "SPI.h"


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
			}else if(mode == UART_MODE)
			{
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

}