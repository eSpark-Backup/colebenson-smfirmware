#include "extern_eeprom.h"


TwoWire* tw;

extern "C"
{
	
	/*********************************************************************************
	 *	@purpose: initializes external EEPROM
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t initEEPROM(int SDA, int SCL)
	{
		tw = new TwoWire(SDA, SCL);
		
		tw->begin();
		pinMode(WP, OUTPUT);
		digitalWrite(WP, LOW);
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose: store Byte
	 *	
	 *  @address: address to store byte
	 *	@value: value to store
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t storeByte(uint16_t address, uint8_t value)
	{
		tw->beginTransmission(ADDRESS_EEPROPM);
		
		tw->write(address >> 8);
		tw->write(address & 0xFF);
		
		tw->write(value);
		
		uint8_t status = tw->endTransmission(false);
		
		return (status ? FAILED : SUCCESS);
	}
	
	/*********************************************************************************
	 *	@purpose: retrieve Byte
	 *	
	 *  @address: address to retrieve byte
	 *	@value: reference of variable for saving retrieved value
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t retrieveByte(uint16_t address, uint8_t* value)
	{
		tw->beginTransmission(ADDRESS_EEPROPM);
		
		tw->write(address >> 8);
		tw->write(address & 0xFF);
		
		tw->endTransmission(false);
		
		uint8_t status = tw->requestFrom(ADDRESS_EEPROPM, 1);
		
		*value = tw->read();
		
		return status ? SUCCESS : FAILED;
	}
}