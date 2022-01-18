#include "TMC5160lib.h"



// Varaibles and structs definitions
SPIClass *mySPI;
uint32_t spi_speed;
VMAX_t vmax_register;
DRVCONF_t DRVCONF_register;
IHOLD_IRUN_t IHOLD_IRUN_register;
SPISettings *settings;

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
			rms_current(600);
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
		settings = new SPISettings(spi_speed, MSBFIRST, SPI_MODE3);
		mySPI->begin();
		
		return SUCCESS;
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
	
	/***********************************************************************************
	 *	@purpose: set RMS current
	 *	
	 *  @mA: current in mA
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static uint8_t rms_current(uint16_t mA) {
	  uint8_t CS = 32.0*1.41421*mA/1000.0*(R_SENSE+0.02)/0.325 - 1;
	  float holdMultiplier = 0.5;
	  // If Current Scale is too low, turn on high sensitivity R_SENSE and calculate again
	  if (CS < 16) {
		vsense(TRUE);
		CS = 32.0*1.41421*mA/1000.0*(R_SENSE+0.02)/0.180 - 1;
	  } else { // If CS >= 16, turn off high_sense_r
		vsense(FALSE);
	  }

	  if (CS > 31)
		CS = 31;

	  irun(CS);
	  ihold(CS*holdMultiplier);
	}
	
	/***********************************************************************************
	 *	@purpose: set value of register address
	 *	
	 *  @address_byte: byte address of register
	 *	@config: config value
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static uint8_t write(uint8_t address_byte, uint32_t config)
	{
	    uint32_t data = (uint32_t)address_byte<<17 | config;
		mySPI->beginTransaction(*settings);
		digitalWrite(CS_PIN, LOW);
		mySPI->transfer((data >> 16) & 0xFF);
		mySPI->transfer((data >>  8) & 0xFF);
		mySPI->transfer(data & 0xFF);
		mySPI->endTransaction();
		digitalWrite(CS_PIN, HIGH);
	}
	
	/***********************************************************************************
	 *	@purpose: vsense settings
	 *	
	 *	@B: boolean
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void vsense(uint8_t B)
	{
		DRVCONF_register.vsense = B;
		write(DRVCONF_register.address, DRVCONF_register.sr);
		
	}
	
	/***********************************************************************************
	 *	@purpose: irun settings
	 *	
	 *	@B: boolean
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
    static void irun(uint8_t B)
	{
		IHOLD_IRUN_register.irun = B;
		write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
		
	}
	
	/***********************************************************************************
	 *	@purpose: ihold settings
	 *	
	 *	@B: boolean
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void ihold(uint8_t B)
	{
		IHOLD_IRUN_register.ihold = B;
		write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
		
	}

}