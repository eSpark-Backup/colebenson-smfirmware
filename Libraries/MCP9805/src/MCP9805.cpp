#include "MCP9805.h"

TwoWire* tw;

extern "C"
{
	/*********************************************************************************
	 *	@purpose: initializes MCP9805 temperature sensor
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	 uint8_t initMCP()
	 {
		
		 
		return FAILED;
	 }
	 
	/*********************************************************************************
	 *	@purpose: get temperature in celsius
	 *	
	 *	@tempC: reference variable to store temperature in C
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	 uint8_t getTempC(float *tempC)
	 {
		
		 
		return FAILED;
	 }
	 
	/*********************************************************************************
	 *	@purpose: get temperature in fahrenhiet
	 *	
	 *	@tempF: reference variable to store temperature in F
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	 uint8_t getTempF(float *tempC)
	 {
		
		 
		return FAILED;
	 }
}