#include "encoder.h"

extern "C"
{
	/*********************************************************************************
	 *	@purpose: initializes Magnetic encoder
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t initEncoder()
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: get updated angle
	 *	
	 *  @dataout: pointer to store 16-bit encocder data
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t getAngle(uint16_t *dataOut)
	{
		return FAILED;
	}
}