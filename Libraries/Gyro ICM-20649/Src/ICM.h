#pragma once

#include "Arduino.h"
#include <Wire.h>

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

// I2C Address
#define ADDRESS_ICM 0x68 

// Default I2C pins
#define SDA PB7
#define SCL PB6

// Registers definitions

// Bank 0
#define ICM20X_B0_WHOAMI 0x00         ///< Chip ID register
#define ICM20X_B0_USER_CTRL 0x03      ///< User Control Reg. Includes I2C Master
#define ICM20X_B0_LP_CONFIG 0x05      ///< Low Power config
#define ICM20X_B0_REG_INT_PIN_CFG 0xF ///< Interrupt config register
#define ICM20X_B0_REG_INT_ENABLE 0x10 ///< Interrupt enable register 0
#define ICM20X_B0_REG_INT_ENABLE_1 0x11 ///< Interrupt enable register 1
#define ICM20X_B0_I2C_MST_STATUS 0x17 ///< Records if I2C master bus data is finished
#define ICM20X_B0_REG_BANK_SEL 0x7F ///< register bank selection register
#define ICM20X_B0_PWR_MGMT_1 0x06   ///< primary power management register
#define ICM20X_B0_ACCEL_XOUT_H 0x2D ///< first byte of accel data
#define ICM20X_B0_GYRO_XOUT_H 0x33  ///< first byte of accel data

// Bank 2
#define ICM20X_B2_GYRO_SMPLRT_DIV 0x00    ///< Gyroscope data rate divisor
#define ICM20X_B2_GYRO_CONFIG_1 0x01      ///< Gyro config for range setting
#define ICM20X_B2_ACCEL_SMPLRT_DIV_1 0x10 ///< Accel data rate divisor MSByte
#define ICM20X_B2_ACCEL_SMPLRT_DIV_2 0x11 ///< Accel data rate divisor LSByte
#define ICM20X_B2_ACCEL_CONFIG_1 0x14     ///< Accel config for setting range

// Bank 3
#define ICM20X_B3_I2C_MST_ODR_CONFIG 0x0 ///< Sets ODR for I2C master bus
#define ICM20X_B3_I2C_MST_CTRL 0x1       ///< I2C master bus config
#define ICM20X_B3_I2C_MST_DELAY_CTRL 0x2 ///< I2C master bus config
#define ICM20X_B3_I2C_SLV0_ADDR 0x3 ///< Sets I2C address for I2C master bus slave 0
#define ICM20X_B3_I2C_SLV0_REG 0x4 ///< Sets register address for I2C master bus slave 0
#define ICM20X_B3_I2C_SLV0_CTRL 0x5 ///< Controls for I2C master bus slave 0
#define ICM20X_B3_I2C_SLV0_DO 0x6   ///< Sets I2C master bus slave 0 data out

#define ICM20X_B3_I2C_SLV4_ADDR 0x13 ///< Sets I2C address for I2C master bus slave 4
#define ICM20X_B3_I2C_SLV4_REG 0x14 ///< Sets register address for I2C master bus slave 4
#define ICM20X_B3_I2C_SLV4_CTRL 0x15 ///< Controls for I2C master bus slave 4
#define ICM20X_B3_I2C_SLV4_DO 0x16   ///< Sets I2C master bus slave 4 data out
#define ICM20X_B3_I2C_SLV4_DI 0x17   ///< Sets I2C master bus slave 4 data in

#define ICM20948_CHIP_ID 0xEA ///< ICM20948 default device id from WHOAMI
#define ICM20649_CHIP_ID 0xE1 ///< ICM20649 default device id from WHOAMI

#define SENSORS_GRAVITY_EARTH (9.80665F) /**< Earth's gravity in m/s^2 */
#define SENSORS_DPS_TO_RADS (0.017453293F) /**< Degrees/s to rad/s multiplier */

// Gyro scale
#define ICM20649_GYRO_RANGE_500_DPS 65.5
#define ICM20649_GYRO_RANGE_1000_DPS 32.8
#define ICM20649_GYRO_RANGE_2000_DPS 16.4
#define ICM20649_GYRO_RANGE_4000_DPS 8.2

// Acc scale
#define ICM20649_ACCEL_RANGE_4_G 8192.0
#define ICM20649_ACCEL_RANGE_8_G 4096.0
#define ICM20649_ACCEL_RANGE_8_G 2048.0
#define ICM20649_ACCEL_RANGE_30_G 1024.0

typedef struct
{
	float x;
	float y;
	float z;
}sensor_t;

extern "C"
{
	// Public functions
	uint8_t initICM();
	uint8_t getGyro(sensor_t* gyro);
	uint8_t getAccel(sensor_t* acc);
	uint8_t getTemp(float* tempC);
	
	// Private functions
	static uint8_t write(uint8_t reg, uint8_t byte, uint8_t stop = false);
	static uint8_t read(uint8_t reg);
	static uint8_t reset();
	static void setBank(uint8_t bankNumber);
	static void sleep(uint8_t state);
	static void writeGyroRange(uint8_t new_gyro_range);
	static void writeAccelRange(uint8_t new_acc_range);
	static void setGyroRateDivisor(uint8_t new_gyro_divisor);
	static void setAccelRateDivisor(uint16_t new_accel_divisor);
	static void readMeasurements(uint8_t reg, uint8_t nBytes, uint8_t* buffer);
}

