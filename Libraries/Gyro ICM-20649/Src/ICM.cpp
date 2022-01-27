#include <ICM.h>

TwoWire tw;

extern "C"
{
	
	/*********************************************************************************
	 *	@purpose: initializes ICM Motion sensor
	 *	
	 *	@SDA: I2C SDA pin
	 *	@SCL: I2C SCL pin
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t initICM(uint32_t SDA, uint32_t SCL)
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

	/*********************************************************************************
	 *	@purpose: get gyro x, y and z readings in rad/s
	 *	
	 *	@gyro: sensor_t struct containing x, y and z members
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t getGyro(sensor_t* gyro)
	{
		uint8_t buffer[14];
		int16_t rawGyroX, rawGyroY, rawGyroZ;
		
		readMeasurements(buffer);
		
		rawGyroX = (((buffer[6] << 8) | buffer[7]));
		rawGyroY = (((buffer[8] << 8) | buffer[9]));
		rawGyroZ = (((buffer[10] << 8) | buffer[11]));
		
		gyro->x = rawGyroX / ICM20649_GYRO_RANGE_4000_DPS * SENSORS_DPS_TO_RADS;
		gyro->y = rawGyroY / ICM20649_GYRO_RANGE_4000_DPS * SENSORS_DPS_TO_RADS;
		gyro->z = rawGyroZ / ICM20649_GYRO_RANGE_4000_DPS * SENSORS_DPS_TO_RADS;
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose: get temperature in celsius
	 *	
	 *	@tempC: reference to float variable to store temperature
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t getTemp(float* tempC)
	{
		uint8_t buffer[14];
		
		readMeasurements(buffer);
		
		*tempC = (buffer[12] << 8) | buffer[13];
		*tempC = *tempC / 333.87 + 21.0;
		
		return SUCCESS;
	}

	/*********************************************************************************
	 *	@purpose: get acceleration x, y and z readings in m/s^2
	 *	
	 *	@tempC: sensor_t struct containing x, y and z members
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/

	uint8_t getAccel(sensor_t* acc)
	{
		uint8_t buffer[14];
		int16_t rawAccX, rawAccY, rawAccZ;
		
		readMeasurements(buffer);
		
		rawAccX = ((buffer[0] << 8) | buffer[1]);
		rawAccY = ((buffer[2] << 8) | buffer[3]);
		rawAccZ = ((buffer[4] << 8) | buffer[5]);
		
		acc->x = rawAccX * SENSORS_GRAVITY_EARTH / ICM20649_ACCEL_RANGE_30_G;
		acc->y = rawAccY * SENSORS_GRAVITY_EARTH / ICM20649_ACCEL_RANGE_30_G;
		acc->z = rawAccZ * SENSORS_GRAVITY_EARTH / ICM20649_ACCEL_RANGE_30_G;
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose: write byte to reg
	 *	
	 *	@reg: sensor register
	 *	@byte: byte to write
	 *	@stop: stop transaction after transmission, true or false
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static uint8_t write(uint8_t reg, uint8_t byte, uint8_t stop)
	{
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(reg);
		tw.write(byte);
		
		tw.endTransmission(stop);
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose:read byte from reg
	 *	
	 *	@reg: sensor register
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static uint8_t read(uint8_t reg)
	{
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(reg);
		tw.endTransmission();
		
		tw.requestFrom(ADDRESS_ICM, 1);
		
		uint8_t res = tw.read();
		
		return res;
	}
	
	
	/*********************************************************************************
	 *	@purpose: reset registers of sensor to default
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static uint8_t reset()
	{
		setBank(0);
		
		uint8_t resetBit = 1 << 7;
		write(ICM20X_B0_PWR_MGMT_1, resetBit);
		
		delay(20);
		
		resetBit = (read(ICM20X_B0_PWR_MGMT_1) >> 7) & 0b1;
		
		while(resetBit)
		{
			resetBit = (read(ICM20X_B0_PWR_MGMT_1) >> 7) & 0b1;
			delay(10);
		}
		
		delay(50);
		
		return SUCCESS;
	}
	
	/*********************************************************************************
	 *	@purpose: set bank number
	 *	
	 *	@bankNumber: bank number 0, 1, 2 or 3
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void setBank(uint8_t bankNumber)
	{
		uint8_t byte = (bankNumber & 0b11) << 4;
		write(ICM20X_B0_REG_BANK_SEL, byte);
	}
	
	/*********************************************************************************
	 *	@purpose: sleep the sensor
	 *	
	 *	@state: true or false
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void sleep(uint8_t state)
	{
		uint8_t byte = (state << 6) | 0x01;
		write(ICM20X_B0_PWR_MGMT_1, byte);
	}
	
	/*********************************************************************************
	 *	@purpose: set gyro range
	 *	
	 *	@new_gyro_range: gyro range to set, refer datasheet for details
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void writeGyroRange(uint8_t new_gyro_range) {
	  setBank(2);
	  
	  uint8_t byte = (new_gyro_range << 1) | 0x01;
	  
	  write(ICM20X_B2_GYRO_CONFIG_1, byte);
	 
	  
	  setBank(0);
	}
	
	/*********************************************************************************
	 *	@purpose: set accel range
	 *	
	 *	@new_accel_range: accel range to set, refer datasheet for details
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void writeAccelRange(uint8_t new_accel_range) {
	  setBank(2);
	  
	  uint8_t byte = (new_accel_range << 1) | 0x01;
	  
	  write(ICM20X_B2_ACCEL_CONFIG_1, byte);
	  
	  setBank(0);
	}
	
	/*********************************************************************************
	 *	@purpose: set gyro rate divisor
	 *	
	 *	@new_gyro_divisor: gyro rate divisor, refer datasheet for details
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void setGyroRateDivisor(uint8_t new_gyro_divisor) {
	  setBank(2);
	  
	  write(ICM20X_B2_GYRO_SMPLRT_DIV, new_gyro_divisor);
	  
	  setBank(0);
	}
	
	/*********************************************************************************
	 *	@purpose: set accel rate divisor
	 *	
	 *	@new_accel_divisor: accel rate divisor, refer datasheet for details
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void setAccelRateDivisor(uint16_t new_accel_divisor) {
	  setBank(2);
	  
	  uint8_t byte_H = (new_accel_divisor >> 8);
	  uint8_t byte_L = (new_accel_divisor) & 0xFF;
	  
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_1, byte_H, true);
	  write(ICM20X_B2_ACCEL_SMPLRT_DIV_2, byte_L);
	  
	  setBank(0);
	}
	
	/*********************************************************************************
	 *	@purpose: read measurements from register
	 *	
	 *	@new_accel_divisor: accel rate divisor, refer datasheet for details
	 *
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	static void readMeasurements(uint8_t* buffer)
	{
		uint8_t numBytes = 14;
		uint8_t count = 0;
		
		setBank(0);
		
		tw.beginTransmission(ADDRESS_ICM);
		
		tw.write(ICM20X_B0_ACCEL_XOUT_H);

		tw.endTransmission(false);
		
		delay(10);

		tw.requestFrom(ADDRESS_ICM, numBytes, true);
		
		while(tw.available())
		{
			buffer[count++] = tw.read();
		}

		
		setBank(0);
		
		
	}

}