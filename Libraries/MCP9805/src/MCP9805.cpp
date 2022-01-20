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
		tw = new TwoWire(SDA_MCP, SCK_MCP);
		
		tw->begin();
		
		
		
		configure();
		
		 
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
		int temp;
		getTemp(&temp);
		
		*tempC = temp * 0.0625;
		
		 
		return SUCCESS;
	 }
	 
	/*********************************************************************************
	 *	@purpose: get temperature in fahrenhiet
	 *	
	 *	@tempF: reference variable to store temperature in F
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	 uint8_t getTempF(float *tempF)
	 {
		int temp;
		getTemp(&temp);
		
		*tempF = (temp * 0.0625) * 1.8 + 32;
		
		 
		return SUCCESS;
	 }
	 
	/*********************************************************************************
	 *	@purpose: configure with configuration register
	 *	
	 *********************************************************************************/
	 
	 static void configure()
	 {
		 tw->beginTransmission(ADDRESS_MCP);
		 
		 //Select configuration register
		 tw->write(CONFIG);
		 //Continuous conversion mode, Power-up result
		 tw->write(0x00);
		 tw->write(0x00);
		 
		 tw->endTransmission();
		 
		 tw->beginTransmission(ADDRESS_MCP);
		 
		 //Select resolution register
		 tw->write(0x08);
		 //Resolution = 0.0625 / C
		 tw->write(0x03);
		 
		 tw->endTransmission();
		 
		 
	 }
	 
	/*********************************************************************************
	 *	@purpose: get raw temperature value 
	 *	
	 *  @temp: reference variable to store value at
	 *********************************************************************************/
	 
	 static void getTemp(int* temp)
	 {
		unsigned int data[2];
		 
		tw->beginTransmission(ADDRESS_MCP);
		 
		//Select data register
		tw->write(DATA);
		
		tw->endTransmission();
		
		tw->requestFrom(ADDRESS_MCP, 2);
		
		if(tw->available() == 2)
		{
			data[0] = tw->read();
			data[1] = tw->read();
		}
		
		// Convert the data to 13-bits
		*temp = ((data[0] & 0x1F) * 256 + data[1]);
		if(*temp > 4095)
		{
			*temp -= 8192;
		}
		
	 }
}