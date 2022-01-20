#include "extern_eeprom.h"


TwoWire* tw;

extern "C"
{
	
	/*********************************************************************************
	 *	@purpose: initializes external EEPROM
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t initEEPROM()
	{
		tw = new TwoWire(SDA_EEPROM, SCK_EEPROM);
		
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
		
		tw->endTransmission(false);
		
		return SUCCESS;
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
		tw->requestFrom(ADDRESS_EEPROPM, 1);
		
		*value = tw->read();
		
		return SUCCESS;
	}
}