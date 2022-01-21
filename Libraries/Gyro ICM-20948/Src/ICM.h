

bool begin_I2C(uint8_t i2c_addr = ICM20649_I2CADDR_DEFAULT, TwoWire *wire = &Wire, int32_t sensor_id = 0); 

bool getAccelRange(void);

void setAccelRange(new_accel_range);

bool getGyroRange(void);

void setGyroRange(new_gyro_range);

bool getEvent(sensors_event_t *);

void getSensor(sensor_t *);

uint8_t getGyroRateDivisor(void);

uint16_t getAccelRateDivisor(void);

bool getEvent(sensors_event_t *accel, sensors_event_t *gyro, sensors_event_t *temp, sensors_event_t *mag = NULL);

