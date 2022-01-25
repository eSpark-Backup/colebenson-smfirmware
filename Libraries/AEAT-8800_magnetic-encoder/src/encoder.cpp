#include "encoder.h"


// Variables, Enums and structs   
SPIClass* mySPI;

static uint16_t newAngle;
static uint8_t _resolution;

char demoText[ 30 ] = {0};

const uint8_t _ANGLE4_REG_CUSTOMER_CFG_0                   = 0x04;
const uint8_t _ANGLE4_REG_CUSTOMER_CFG_1                   = 0x05;
const uint8_t _ANGLE4_REG_CUSTOMER_CFG_2                   = 0x06;
const uint8_t _ANGLE4_REG_OPERATING_VOLTAGE                = 0x0A;
const uint8_t _ANGLE4_REG_UNLOCK_REGISTER                  = 0x10;
const uint8_t _ANGLE4_REG_CUSTOMER_RESERVED_OTP            = 0x11;
const uint8_t _ANGLE4_REG_ST_ZERO_RESET_OTP                = 0x12;
const uint8_t _ANGLE4_REG_CUSTOMER_CONFIG_OTP              = 0x13;
const uint8_t _ANGLE4_REG_PROGRAM_OPERATING_VOLTAGE        = 0x14;
const uint8_t _ANGLE4_REG_CONFIG_OF_FULL_CALIBRATION_OTP   = 0x1B;

/* Customer Configuration 0 */
const uint8_t _ANGLE4_CCFG0_SELECT_UVW_MODE = 0x80;
const uint8_t _ANGLE4_CCFG0_SELECT_PWM_MODE = 0x00;
const uint8_t _ANGLE4_CCFG0_PWM_PERIOD_8193us = 0x60;
const uint8_t _ANGLE4_CCFG0_PWM_PERIOD_4097us = 0x40;
const uint8_t _ANGLE4_CCFG0_PWM_PERIOD_2049us = 0x20;
const uint8_t _ANGLE4_CCFG0_PWM_PERIOD_1025us = 0x00;
const uint8_t _ANGLE4_CCFG0_ABI_360edeg       = 0x18;
const uint8_t _ANGLE4_CCFG0_ABI_270edeg       = 0x10;
const uint8_t _ANGLE4_CCFG0_ABI_180edeg       = 0x08;
const uint8_t _ANGLE4_CCFG0_ABI_90edeg        = 0x00;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_8  = 0x07;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_7  = 0x06;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_6  = 0x05;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_5  = 0x04;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_4  = 0x03;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_3  = 0x02;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_2  = 0x01;
const uint8_t _ANGLE4_CCFG0_UVW_POLE_PAIRS_1  = 0x00;

/* Customer Configuration 1 */
const uint8_t _ANGLE4_CCFG1_ABI_CPR_4096         = 0x70;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_2048         = 0x60;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_1024         = 0x50;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_512          = 0x40;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_256          = 0x30;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_128          = 0x20;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_64           = 0x10;
const uint8_t _ANGLE4_CCFG1_ABI_CPR_32           = 0x00;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_1_4mdeg   = 0x09;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_7mdeg   = 0x08;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_35mdeg  = 0x07;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_17mdeg  = 0x06;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_08mdeg  = 0x05;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_04mdeg  = 0x04;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_02mdeg  = 0x03;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_01mdeg  = 0x02;
const uint8_t _ANGLE4_CCFG1_HYSTERESIS_0_005mdeg = 0x01;
const uint8_t _ANGLE4_CCFG1_NO_HYSTERESIS        = 0x00;

/* Customer Configuration 2 */
const uint8_t _ANGLE4_CCFG2_DIR_COUNTER_CLOCKWISE_ROTATION  = 0x80;
const uint8_t _ANGLE4_CCFG2_DIR_CLOCKWISE_ROTATION          = 0x00;
const uint8_t _ANGLE4_CCFG2_ZERO_LATENCY_MODE_ON            = 0x40;
const uint8_t _ANGLE4_CCFG2_ZERO_LATENCY_MODE_OFF           = 0x00;
const uint8_t _ANGLE4_CCFG2_ABS_RESOLUTION_10bit            = 0x30;
const uint8_t _ANGLE4_CCFG2_ABS_RESOLUTION_12bit            = 0x20;
const uint8_t _ANGLE4_CCFG2_ABS_RESOLUTION_14bit            = 0x10;
const uint8_t _ANGLE4_CCFG2_ABS_RESOLUTION_16bit            = 0x00;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_4096               = 0x06;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_2048               = 0x06;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_1024               = 0x04;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_512                = 0x04;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_256                = 0x03;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_128                = 0x02;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_64                 = 0x02;
const uint8_t _ANGLE4_CCFG2_SETTINGS_CPR_32                 = 0x02;
const uint8_t _ANGLE4_CCFG2_CPR_ABS_RESOLUTION              = 0x04;

/* Operating Voltage */
const uint8_t _ANGLE4_OPERATING_VOLTAGE_3300mV   = 0x00;
const uint8_t _ANGLE4_OPERATING_VOLTAGE_5000mV   = 0x02;

/* Absolute Resolution */
const uint8_t _ANGLE4_DATA_RESOLUTION_14bit = 0x10;
const uint8_t _ANGLE4_DATA_RESOLUTION_16bit = 0x00;
const uint8_t _ANGLE4_DATA_RESOLUTION_12bit = 0x20;
const uint8_t _ANGLE4_DATA_RESOLUTION_10bit = 0x30;

extern "C"
{
	/*********************************************************************************
	 *	@purpose: initializes magnetic encoder
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t initEncoder(uint32_t MOSI, uint32_t MISO, uint32_t SCK, uint32_t CS)
	{
		pinMode(CS, OUTPUT);
		mySPI = new SPIClass(MOSI, MISO, SCK);
		mySPI->beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE3));
		
		applicationInit();
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose: get updated angle
	 *	
	 *  @dataout: pointer to store 16-bit encocder data
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t getAngle(uint16_t *dataOut)
	{
		uint8_t status;
		status = angle4_getNewAngle(dataOut);
		
		return status;
	}
	
	/*********************************************************************************
	 *	@purpose: init application
	 *	
	 *********************************************************************************/
	
	static void applicationInit()
	{
	  angle4_startMesuremenet();

	}
	
	/*********************************************************************************
	 *	@purpose: start angle measurement
	 *	
	 *********************************************************************************/
	
	static void angle4_startMesuremenet()
	{
	  uint8_t writeData[ 1 ];

	  writeData[ 0 ] = 0x80;

	  digitalWrite(EN_CS, LOW);
	  mySPI->transfer(writeData, 1);
	  digitalWrite(EN_CS, HIGH);
	}
	
	/*********************************************************************************
	 *	@purpose: get angle
	 *	
	 *********************************************************************************/
	
	static uint8_t angle4_getNewAngle(uint16_t *dataOut)
	{
	  uint8_t readReg[ 3 ] = { 0, 0, 0 };
	  uint16_t angleData;
	  uint8_t status;
	  
	  digitalWrite(EN_CS, HIGH);
	  mySPI->transfer(readReg, 3);
	  digitalWrite(EN_CS, LOW);

	  angleData = readReg[0];
	  angleData = angleData << 8;
	  angleData = angleData | readReg[1];

	  status = readReg[2] >> 4;

	  if (_resolution == _ANGLE4_DATA_RESOLUTION_14bit)
	  {
		Serial.println("14bit");
		angleData = angleData >> 2;
		newAngle = angleData * 0.02197;

		status = (readReg[1] & 0x03) << 2;
		status |= (readReg[2] & 0xC0) >> 6;
	  }
	  else if (_resolution == _ANGLE4_DATA_RESOLUTION_12bit)
	  {
		Serial.println("12bit");
		angleData = angleData >> 4;
		newAngle = angleData * 0.087890;
		
		status = readReg[1] & 0x0F;
	  }
	  else if (_resolution == _ANGLE4_DATA_RESOLUTION_10bit)
	  {
		angleData = angleData >> 6;
		newAngle = angleData * 0.35156;
		
		status = (readReg[1] >> 2) & 0x0F;
	  }
	  else
	  {
		Serial.println("16-bit");
		newAngle = angleData * 0.005493247;
		
		status = (readReg[2] >> 4) & 0x0F;
	  }

	  *dataOut = newAngle;
	  
	  if(status < 8)
	  {
		  status = FAILED;
	  }else if(status >= 8)
	  {
		  status = SUCCESS;
	  }
	  
	  return status;
	}
}