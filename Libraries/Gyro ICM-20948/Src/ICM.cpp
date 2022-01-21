#include <ICM.h>


#define ICM20649_I2CADDR_DEFAULT 0x68

bool begin_I2C(uint8_t i2c_address, TwoWire *wire, int32_t sensor_id) {

  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(ICM20649_I2CADDR_DEFAULT 0x68, wire);

  if (!i2c_dev->begin()) {
    Serial.println("I2C begin Failed");
    return false;
  }

  return _init(sensor_id);
}


void setAccelRange(bool new_accel_range) {
  writeAccelRange((uint8_t)new_accel_range);
}

bool getAccelRange(void) {
  return (icm20649_accel_range_t)readAccelRange() ;
}

setGyroRange(new_gyro_range) {
  writeGyroRange((uint8_t)new_gyro_range);
}
  
getGyroRange(void) {
  return (icm20649_gyro_range_t)readGyroRange();
}

