#include "TMC5160lib.h"
#include "Arduino.h"
#include "SPI.h"
/*
 *	@purpose: Initializes TMC Stepper driver based on mode selected i.e. SPI or UART
 *	
 *	@mode: SPI or UART
 *	@return: status, SUCCESS or FAILED
*/

extern "C"
{
	uint8_t initTMCSPI(uint8_t MODE)
		{
			if(MODE == SPI_mode)
			{
			}else if(MODE == UART_mode)
			{
			}
			
			return FAILED;
		}

}