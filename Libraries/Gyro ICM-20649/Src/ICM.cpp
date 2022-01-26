#include <ICM.h>

TwoWire tw;

extern "C"
{

	uint8_t initICM()
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
		uint8_t buffer[3];
		
		readMeasurements(ICM20X_B0_GYRO_XOUT_H, 3, buffer);
		
		gyro->x = buffer[0];
		gyro->y = buffer[1];
		gyro->z = buffer[2];
		
		return SUCCESS;
	}

	uint8_t getAccel(sensor_t* acc)
	{
		uint8_t buffer[3];
		
		readMeasurements(ICM20X_B0_ACCEL_XOUT_H, 3, buffer);
		
		acc->x = buffer[0];
		acc->y = buffer[1];
		acc->z = buffer[2];
		
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
	
	static void writeAccelRange(uint8_t new_accel_range) {
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
	  setBank(2);
	  
	  uint8_t byte_H = (new_accel_divisor >> 8);
	  uint8_t byte_L = (new_accel_divisor) & 0xFF;
	  
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_1, byte_H, true);
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_2, byte_L);
	  
	  setBank(0);
	}
	
	static void readMeasurements(uint8_t reg, uint8_t nBytes, uint8_t* buffer)
	{
		uint8_t numBytes = nBytes;
		uint8_t count = 0;
		
		setBank(0);
		
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(reg);
		
		tw.endTransmission(true);
		
		tw.requestFrom(ADDRESS_ICM, numBytes);
		
		while((tw.available() > 0))
		{
			buffer[count++] = tw.read();
		}
		
		setBank(0);
		
		
	}

}