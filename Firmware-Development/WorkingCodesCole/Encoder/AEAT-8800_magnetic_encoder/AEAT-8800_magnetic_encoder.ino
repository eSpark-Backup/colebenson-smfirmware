#include <SPI.h>

static uint16_t newAngle;
static uint8_t _resolution;

uint16_t Angle;
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




SPIClass mySPI(PB15, PB14, PB13);


uint8_t angle4_readByte( uint8_t addr )
{
  uint8_t writeData[ 2 ] = {0};
  //uint8_t readData[ 2 ] = {0};

  writeData[ 0 ] = addr | 0x80;

  digitalWrite(PB12, HIGH);  //hal_gpio_csSet( 0 );
  mySPI.transfer(writeData, 2);  //hal_spiTransfer(writeData, readData, 2);
  digitalWrite(PB12, LOW); //hal_gpio_csSet( 1 );

  return writeData[ 1 ];
}


void angle4_writeByte(uint8_t addr, uint8_t _data)
{
  uint8_t writeData[ 2 ];

  writeData[ 0 ] = addr | 0x40;
  writeData[ 1 ] = _data;

  digitalWrite(PB12, LOW);    //hal_gpio_csSet( 0 );
  mySPI.transfer(writeData, 2); //hal_spiWrite(writeData, 2);
  digitalWrite(PB12, HIGH);   //hal_gpio_csSet( 1 );
}


void angle4_startMesuremenet()
{
  uint8_t writeData[ 1 ];

  writeData[ 0 ] = 0x80;

  digitalWrite(PB12, LOW);  //hal_gpio_csSet( 0 );
  mySPI.transfer(writeData, 1);  //hal_spiWrite( writeData, 1 );
  digitalWrite(PB12, HIGH);  //hal_gpio_csSet( 1 );
}

uint8_t angle4_getNewAngle(uint16_t *dataOut)
{
  uint8_t readReg[ 3 ] = { 0, 0, 0 };
  uint16_t angleData;
  uint8_t status;
  //Serial.print("Status before ");
  //Serial.println(readReg[2]);
  digitalWrite(PB12, HIGH);  //hal_gpio_csSet( 1 );
  mySPI.transfer(readReg, 3); //hal_spiRead( readReg, 3 );
  digitalWrite(PB12, LOW); //hal_gpio_csSet( 0 );

  angleData = readReg[0];
  angleData = angleData << 8;
  angleData = angleData | readReg[1];
  //Serial.print("Status after ");
  //Serial.println(readReg[2]);
  /* Status 0x0F - Ready, MHI, MLO, Parity */
  status = readReg[2] >> 4;

  if (_resolution == _ANGLE4_DATA_RESOLUTION_14bit)
  {
    Serial.println("14bit");
    angleData = angleData >> 2;
    newAngle = angleData * 0.02197;
    /* Status 0x0F - Ready, MHI, MLO, Parity */
    Serial.print("REG[0]");
    Serial.println(readReg[0], HEX);
    Serial.print("REG[1]:");
    Serial.println(readReg[1], HEX);
    Serial.print("REG[2]:");
    Serial.println(readReg[2], HEX);

    status = (readReg[1] & 0x03) << 2;
    status |= (readReg[2] & 0xC0) >> 6;
  }
  else if (_resolution == _ANGLE4_DATA_RESOLUTION_12bit)
  {
    Serial.println("12bit");
    angleData = angleData >> 4;
    newAngle = angleData * 0.087890;
    /* Status 0x0F - Ready, MHI, MLO, Parity */
    Serial.print("REG[0]");
    Serial.println(readReg[0], HEX);
    Serial.print("REG[1]:");
    Serial.println(readReg[1], HEX);
    Serial.print("REG[2]:");
    Serial.println(readReg[2], HEX);
    status = readReg[1] & 0x0F;
  }
  else if (_resolution == _ANGLE4_DATA_RESOLUTION_10bit)
  {
    angleData = angleData >> 6;
    newAngle = angleData * 0.35156;
    /* Status 0x0F - Ready, MHI, MLO, Parity */
    status = (readReg[1] >> 2) & 0x0F;
  }
  else
  {
    Serial.println("16-bit");
    newAngle = angleData * 0.005493247;
    /* Status 0x0F - Ready, MHI, MLO, Parity */
    status = (readReg[2] >> 4) & 0x0F;
  }

  *dataOut = newAngle;
  return status;
}


void angle4_calibration(uint8_t dir, uint8_t data_resolution)
{
  _resolution = data_resolution;

  angle4_writeByte(_ANGLE4_REG_CUSTOMER_CFG_2,  dir | data_resolution | _ANGLE4_CCFG2_SETTINGS_CPR_4096 );

  Serial.print("_ANGLE4_REG_CUSTOMER_CFG_2: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_CFG_2));

  angle4_writeByte( _ANGLE4_REG_UNLOCK_REGISTER, 0xAB);

  Serial.print("_ANGLE4_REG_UNLOCK_REGISTER: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_UNLOCK_REGISTER));

  angle4_writeByte( _ANGLE4_REG_CUSTOMER_RESERVED_OTP, 0xA1);

  Serial.print("_ANGLE4_REG_CUSTOMER_RESERVED_OTP: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_RESERVED_OTP));

  angle4_writeByte( _ANGLE4_REG_ST_ZERO_RESET_OTP, 0xA2);

  Serial.print("_ANGLE4_REG_ST_ZERO_RESET_OTP: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_ST_ZERO_RESET_OTP));

  angle4_writeByte( _ANGLE4_REG_CUSTOMER_CONFIG_OTP, 0xA3);

  Serial.print("_ANGLE4_REG_CUSTOMER_CONFIG_OTP: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_CONFIG_OTP));

  angle4_writeByte( _ANGLE4_REG_PROGRAM_OPERATING_VOLTAGE, 0xA4);

  Serial.print("_ANGLE4_REG_PROGRAM_OPERATING_VOLTAGE: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_PROGRAM_OPERATING_VOLTAGE));

  angle4_writeByte( _ANGLE4_REG_CONFIG_OF_FULL_CALIBRATION_OTP, 0xA5);

  Serial.print("_ANGLE4_REG_CONFIG_OF_FULL_CALIBRATION_OTP: ");
  Serial.println(angle4_readByte(_ANGLE4_REG_CONFIG_OF_FULL_CALIBRATION_OTP));

  angle4_writeByte(0x00, 0xAA);

  Serial.print("0x00: ");
  Serial.println(angle4_readByte(0x00));
}


void applicationInit()
{

  /*angle4_writeByte(_ANGLE4_REG_CUSTOMER_CFG_0, _ANGLE4_CCFG0_SELECT_UVW_MODE |
                   _ANGLE4_CCFG0_PWM_PERIOD_1025us |
                   _ANGLE4_CCFG0_ABI_270edeg |
                   _ANGLE4_CCFG0_UVW_POLE_PAIRS_8 ); // DEC value 151*/

  //  angle4_writeByte(_ANGLE4_REG_CUSTOMER_CFG_0, 151 ); // DEC value 151






  /*angle4_writeByte(_ANGLE4_REG_CUSTOMER_CFG_1, _ANGLE4_CCFG1_ABI_CPR_4096 |
                   _ANGLE4_CCFG1_HYSTERESIS_1_4mdeg ); // DEC value 121*/



  //angle4_calibration(_ANGLE4_CCFG2_DIR_COUNTER_CLOCKWISE_ROTATION, _ANGLE4_CCFG2_ABS_RESOLUTION_12bit);

  angle4_startMesuremenet();


}


void setup() {
  Serial.begin(115200);
  pinMode(PB12, OUTPUT);
  //  mySPI.setDataMode(SPI_MODE3);
  //  mySPI.setClockDivider(SPI_CLOCK_DIV128);
  //  mySPI.setBitOrder(MSBFIRST);
  mySPI.beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE3));
  delay(1000);



  /* uint8_t x[2];
    x[0] = 0x06;
    x[1] = 0x04;*/
  //
  //    mySPI.transfer(0x10);
  //    mySPI.transfer(0xAB);

  applicationInit();
  //delay(500);

}

void loop() {
  //Serial.print("_ANGLE4_REG_CUSTOMER_CFG_0: ");
  //Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_CFG_0));
  uint8_t status;
  status = angle4_getNewAngle(&Angle);
  //Serial.print("_ANGLE4_REG_CUSTOMER_CFG_1: ");
  //Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_CFG_1));

  Serial.print("status = ");
  Serial.println(status);
  Serial.print("angle = ");
  Serial.println(Angle);
  delay(100);












  /*  uint8_t readReg[ 3 ] = { 0 };
    uint16_t angleData;
    uint8_t status;

  /*  uint8_t readReg[ 3 ] = { 0 };
    uint16_t angleData;
    uint8_t status;

    digitalWrite(PB12, HIGH);
    mySPI.transfer(readReg, 3);
    digitalWrite(PB12, LOW);

    angleData = readReg[0];
    angleData = angleData << 8;
    angleData = angleData | readReg[0];

    status = readReg[2] >> 4;

    Serial.print("Angle: ");
    Serial.println(angleData);

    Serial.print("Status: ");
    Serial.println(status);

    delay(100);*/
}
