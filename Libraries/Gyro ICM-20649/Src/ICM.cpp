#include <ICM.h>

TwoWire tw;

extern "C"
{

	uint8_t initGyro()
	{
		tw.setSDA(SDA);
		tw.setSCL(SCL);
		
		tw.begin();
		
		uint8_t id = read(ICM20X_B0_WHOAMI);
		
		if(id != ICM20649_CHIP_ID)
		{
			return false;
		}
		
		reset();
		
		sleep(false);
		
		writeGyroRange(3);
		writeAccelRange(3);
		
		// 1100Hz/(1+10) = 100Hz
		setGyroRateDivisor(10);
		
		// # 1125Hz/(1+20) = 53.57Hz
		setAccelRateDivisor(20);
		
		delay(20);
		
		return SUCCESS;
	}

	uint8_t getGyro(sensor_t* gyro)
	{
		return SUCCESS;
	}

	uint8_t getAccel(sensor_t* acc)
	{
		return SUCCESS;
	}
	
	static uint8_t write(uint8_t reg, uint8_t byte, uint8_t stop)
	{
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(reg);
		tw.write(byte);
		
		tw.endTransmission(stop);
		
		return SUCCESS;
	}
	
	static uint8_t read(uint8_t reg)
	{
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(reg);
		tw.requestFrom(ADDRESS_ICM, 1);
		
		uint8_t res = tw.read();
		
		tw.endTransmission();
		
		return res;
	}
	
	static uint8_t reset()
	{
		setBank(0);
		
		uint8_t resetBit = 1 << 7;
		write(ICM20X_B0_PWR_MGMT_1, resetBit);
		
		delay(20);
		
		resetBit = (read(ICM20X_B0_PWR_MGMT_1) >> 7) & 0b1;
		
		while(resetBit)
		{
			delay(10);
		}
		
		delay(50);
		
		return SUCCESS;
	}
	
	static void setBank(uint8_t bankNumber)
	{
		uint8_t byte = (bankNumber & 0b11) << 4;
		write(ICM20X_B0_REG_BANK_SEL, byte);
	}
	
	static void sleep(uint8_t state)
	{
		uint8_t byte = state << 6;
		write(ICM20X_B0_PWR_MGMT_1, byte);
	}
	
	static void writeGyroRange(uint8_t new_gyro_range) {
	  setBank(2);
	  
	  uint8_t byte = new_gyro_range << 1;
	  write(ICM20X_B2_GYRO_CONFIG_1, byte);
	  
	  setBank(0);
	}
	
	static void writeAccelRange(uint8_t new_accel_ange) {
	  setBank(2);
	  
	  uint8_t byte = new_accel_range << 1;
	  write(ICM20X_B2_ACCEL_CONFIG_1, byte);
	  
	  setBank(0);
	}
	
	static void setGyroRateDivisor(uint8_t new_gyro_divisor) {
	  setBank(2);
	  
	  write(ICM20X_B2_GYRO_SMPLRT_DIV, new_gyro_divisor);
	  
	  setBank(0);
	}
	
	static void setAccelRateDivisor(uint16_t new_accel_divisor) {
	  _setBank(2);

	  Adafruit_BusIO_Register accel_rate_divisor =
		  Adafruit_BusIO_Register(i2c_dev, spi_dev, ADDRBIT8_HIGH_TOREAD,
								  ICM20X_B2_ACCEL_SMPLRT_DIV_1, 2, MSBFIRST);

	  accel_rate_divisor.write(new_accel_divisor);
	  
	  uint8_t byte_H = (new_accel_divisor >> 8);
	  uint8_t byte_L = (new_accel_divisor) & 0xFF;
	  
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_1, byte_H, true);
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_2, byte_L);
	  
	  _setBank(0);
	}

}