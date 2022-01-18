#include "TMC5160lib.h"



// Varaibles and structs definitions
SPIClass *mySPI;
uint32_t spi_speed;
VMAX_t vmax_register;
DRVCONF_t DRVCONF_register;
IHOLD_IRUN_t IHOLD_IRUN_register;
GCONF_t GCONF_register;
PWMCONF_t PWMCONF_register;
SPISettings *settings;
CHOPCONF_t CHOPCONF_register;
GLOBAL_SCALER_t GLOBAL_SCALER_register;
RAMPMODE_t RAMPMODE_register;
VSTART_t VSTART_register;
A1_t A1_register;
V1_t V1_register;
AMAX_t AMAX_register;
VMAX_t VMAX_register;
DMAX_t DMAX_register;
D1_t D1_register;
VSTOP_t VSTOP_register;
XACTUAL_t XACTUAL_register;

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
			en_pwm_mode(TRUE);
			pwm_autoscale(TRUE);
			microsteps(128);
			toff(1);
			irun(17);
			ihold(10);
			iholddelay(6);
			GLOBAL_SCALER(130);
			//ramp settings
			RAMPMODE(0);
			VSTART(0);
			a1(5);
			v1(40);
			AMAX(1000);
			VMAX(100000);
			DMAX(300);
			d1(40);
			VSTOP(0);
			shaft(NOT_INVERT);

			//current motor position
			XACTUAL(0);
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
	
	/***********************************************************************************
	 *	@purpose: en_pwm_mode settings
	 *	
	 *	@B: boolean
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void en_pwm_mode(uint8_t B)
	{
		GCONF_register.en_pwm_mode = B;
		write(GCONF_register.address, GCONF_register.sr);
		
	}
	
	/***********************************************************************************
	 *	@purpose: pwm_autoscale settings
	 *	
	 *	@B: boolean
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void pwm_autoscale(uint8_t B)
	{
		PWMCONF_register.pwm_autoscale = B;
		write(PWMCONF_register.address, PWMCONF_register.sr);
	}
	
	static void mres(uint8_t B)
	{
		CHOPCONF_register.mres = B;
		write(CHOPCONF_register.address, CHOPCONF_register.sr);
	}
	
	static void toff(uint8_t B)
	{
		CHOPCONF_register.toff = B;
		write(CHOPCONF_register.address, CHOPCONF_register.sr);
	}
	
	static void iholddelay(uint8_t B)
	{
		IHOLD_IRUN_register.iholddelay = B;
		write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
		
	}
	
	static void GLOBAL_SCALER(uint8_t input) {
	  GLOBAL_SCALER_register.sr = input;
	  write(GLOBAL_SCALER_register.address, GLOBAL_SCALER_register.sr);
	}
	
	static void RAMPMODE(uint8_t input) {
	  RAMPMODE_register.sr = input;
	  write(RAMPMODE_register.address, RAMPMODE_register.sr);
	}
	
	static void VSTART(uint32_t input) {
	  VSTART_register.sr = input;
	  write(VSTART_register.address, VSTART_register.sr);
	}
	
	static void a1(uint16_t input) {
	  A1_register.sr = input;
	  write(A1_register.address, A1_register.sr);
	}
	
	static void v1(uint32_t input) {
	  V1_register.sr = input;
	  write(V1_register.address, V1_register.sr);
	}
	
	static void AMAX(uint16_t input) {
	  AMAX_register.sr = input;
	  write(AMAX_register.address, AMAX_register.sr);
	}
	
	static void VMAX(uint32_t input) {
	  VMAX_register.sr = input;
	  write(VMAX_register.address, VMAX_register.sr);
	}
	
	static void DMAX(uint16_t input) {
	  DMAX_register.sr = input;
	  write(DMAX_register.address, DMAX_register.sr);
	}
	
	static void d1(uint16_t input) {
	  D1_register.sr = input;
	  write(D1_register.address, D1_register.sr);
	}
	
	static void VSTOP(uint32_t input) {
	  //if (input == 0 && RAMPMODE() == 0) return;
	  VSTOP_register.sr = input;
	  write(VSTOP_register.address, VSTOP_register.sr);
	}
	
	static void shaft(uint8_t B) {
	  GCONF_register.shaft = B;
	  write(GCONF_register.address, GCONF_register.sr);
	}
	
	static void XACTUAL(int32_t input) {
	  write(XACTUAL_register.address, input);
	}
	
	static void microsteps(uint16_t ms) {
	  switch(ms) {
		case 256: mres(0); break;
		case 128: mres(1); break;
		case  64: mres(2); break;
		case  32: mres(3); break;
		case  16: mres(4); break;
		case   8: mres(5); break;
		case   4: mres(6); break;
		case   2: mres(7); break;
		case   0: mres(8); break;
		default: break;
	  }
	}
}

