#include <stdio.h>
#include <Arduino.h>
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>

#define EN_PIN           PC11 // Enable
#define CS_PIN           PA4
#define MOSI_PIN         PA7
#define MISO_PIN         PA6
#define SCK_PIN          PA5
#define R_SENSE          0.0165f
#define PIN        PB3 // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 1 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_ICM20649 icm;

TwoWire I2C(PB11, PB10);
TwoWire I2C_EEPROM(PC9, PA8);
SPIClass mySPI(PB15, PB14, PB13);

#define Wire I2C
#define Wire_EE I2C_EEPROM

// MCP9805 I2C address is 0x18(24)
#define MCP9805_Addr 0x18
#define EEPROM_Addr 0x50
static uint16_t newAngle;
static uint8_t _resolution;

uint16_t Angle;
uint8_t mem_addr = 0;

/*HardwareSerial hserial2(PA1, PA0); // (RX, TX)
  #define Serial hserial2*/

#define hserial2 Serial

TMC5160Stepper driver = TMC5160Stepper(CS_PIN, R_SENSE, MOSI_PIN, MISO_PIN, SCK_PIN);

bool dir = true;
bool shaft = false;
uint32_t speed;


#define DEBUG 1

/* Symbolic names for bit rate of CAN message                                */
typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

typedef struct
{
  uint32_t id;        /* 29 bit identifier                               */
  uint8_t  data[8];   /* Data field                                     */
  uint8_t  len;       /* Length of data field in bytes                   */
  uint8_t  ch;        /* Object channel(Not use)                         */
  uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

typedef const struct
{
  uint8_t TS2;
  uint8_t TS1;
  uint8_t BRP;
} CAN_bit_timing_config_t;

CAN_bit_timing_config_t can_configs[6] = {{2, 12, 56}, {2, 12, 28}, {2, 13, 21}, {2, 11, 12}, {2, 11, 6}, {1, 5, 6}};

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

/**
   Print registers.
*/
void printRegister(char * buf, uint32_t reg) {
  if (DEBUG == 0) return;
  Serial.print(buf);
  Serial.print(reg, HEX);
  Serial.println();
}

/**
   Initializes the CAN GPIO registers.

   @params: addr    - Specified GPIO register address.
   @params: index   - Specified GPIO index.
   @params: speed   - Specified OSPEEDR register value.(Optional)

*/
void CANSetGpio(GPIO_TypeDef * addr, uint8_t index, uint8_t speed = 3) {
  uint8_t _index2 = index * 2;
  uint8_t _index4 = index * 4;
  uint8_t ofs = 0;
  uint8_t setting;

  if (index > 7) {
    _index4 = (index - 8) * 4;
    ofs = 1;
  }

  uint32_t mask;
  printRegister("GPIO_AFR(b)=", addr->AFR[1]);
  mask = 0xF << _index4;
  addr->AFR[ofs]  &= ~mask;         // Reset alternate function
  setting = 0x9;                    // AF9
  mask = setting << _index4;
  addr->AFR[ofs]  |= mask;          // Set alternate function
  printRegister("GPIO_AFR(a)=", addr->AFR[1]);

  printRegister("GPIO_MODER(b)=", addr->MODER);
  mask = 0x3 << _index2;
  addr->MODER   &= ~mask;           // Reset mode
  setting = 0x2;                    // Alternate function mode
  mask = setting << _index2;
  addr->MODER   |= mask;            // Set mode
  printRegister("GPIO_MODER(a)=", addr->MODER);

  printRegister("GPIO_OSPEEDR(b)=", addr->OSPEEDR);
  mask = 0x3 << _index2;
  addr->OSPEEDR &= ~mask;           // Reset speed
  setting = speed;
  mask = setting << _index2;
  addr->OSPEEDR |= mask;            // Set speed
  printRegister("GPIO_OSPEEDR(a)=", addr->OSPEEDR);

  printRegister("GPIO_OTYPER(b)=", addr->OTYPER);
  mask = 0x1 << index;
  addr->OTYPER  &= ~mask;           // Reset Output push-pull
  printRegister("GPIO_OTYPER(a)=", addr->OTYPER);

  printRegister("GPIO_PUPDR(b)=", addr->PUPDR);
  mask = 0x3 << _index2;
  addr->PUPDR   &= ~mask;           // Reset port pull-up/pull-down
  printRegister("GPIO_PUPDR(a)=", addr->PUPDR);
}



/**
   Initializes the CAN filter registers.

   @preconditions   - This register can be written only when the filter initialization mode is set (FINIT=1) in the CAN_FMR register.
   @params: index   - Specified filter index. index 27:14 are available in connectivity line devices only.
   @params: scale   - Select filter scale.
                      0: Dual 16-bit scale configuration
                      1: Single 32-bit scale configuration
   @params: mode    - Select filter mode.
                      0: Two 32-bit registers of filter bank x are in Identifier Mask mode
                      1: Two 32-bit registers of filter bank x are in Identifier List mode
   @params: fifo    - Select filter assigned.
                      0: Filter assigned to FIFO 0
                      1: Filter assigned to FIFO 1
   @params: bank1   - Filter bank register 1
   @params: bank2   - Filter bank register 2

*/
void CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2) {
  if (index > 27) return;

  CAN1->FA1R &= ~(0x1UL << index);             // Deactivate filter

  if (scale == 0) {
    CAN1->FS1R &= ~(0x1UL << index);           // Set filter to Dual 16-bit scale configuration
  } else {
    CAN1->FS1R |= (0x1UL << index);            // Set filter to single 32 bit configuration
  }
  if (mode == 0) {
    CAN1->FM1R &= ~(0x1UL << index);           // Set filter to Mask mode
  } else {
    CAN1->FM1R |= (0x1UL << index);            // Set filter to List mode
  }

  if (fifo == 0) {
    CAN1->FFA1R &= ~(0x1UL << index);          // Set filter assigned to FIFO 0
  } else {
    CAN1->FFA1R |= (0x1UL << index);           // Set filter assigned to FIFO 1
  }

  CAN1->sFilterRegister[index].FR1 = bank1;    // Set filter bank registers1
  CAN1->sFilterRegister[index].FR2 = bank2;    // Set filter bank registers2

  CAN1->FA1R |= (0x1UL << index);              // Activate filter

}

/**
   Initializes the CAN controller with specified bit rate.

   @params: bitrate - Specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
   @params: remap   - Select CAN port.
                      =0:CAN1_RX mapped to PA11, CAN1_TX mapped to PA12
                         CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6
                      =1:Not used
                      =2:CAN1_RX mapped to PB8,  CAN1_TX mapped to PB9 (not available on 36-pin package)
                         CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
                      =3:CAN1_RX mapped to PD0,  CAN1_TX mapped to PD1 (available on 100-pin and 144-pin package)
                         CAN2_RX mapped to PB12, CAN2_TX mapped to PB13

*/
bool CANInit(BITRATE bitrate, int remap)
{
  // Reference manual
  // https://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf

  RCC->APB1ENR |= 0x2000000UL;           // Enable CAN1 clock
  RCC->APB1ENR |= 0x4000000UL;           // Enable CAN2 clock

  if (remap == 0) {
    // CAN1
    RCC->AHB1ENR |= 0x1;                 // Enable GPIOA clock
    CANSetGpio(GPIOA, 11);               // Set PA11
    CANSetGpio(GPIOA, 12);               // Set PA12

    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock
    CANSetGpio(GPIOB, 5);                // Set PB5
    CANSetGpio(GPIOB, 6);                // Set PB6
  }

  if (remap == 2) {
    // CAN1
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock
    CANSetGpio(GPIOB, 8);                // Set PB8
    CANSetGpio(GPIOB, 9);                // Set PB9

    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock
    CANSetGpio(GPIOB, 12);               // Set PB12
    CANSetGpio(GPIOB, 13);               // Set PB13
  }

  if (remap == 3) {
    // CAN1
    RCC->AHB1ENR |= 0x8;                 // Enable GPIOD clock
    CANSetGpio(GPIOD, 0);                // Set PD0
    CANSetGpio(GPIOD, 1);                // Set PD1

    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock
    CANSetGpio(GPIOB, 12);               // Set PB12
    CANSetGpio(GPIOB, 13);               // Set PB13
  }

  CAN1->MCR |= 0x1UL;                    // Require CAN1 to Initialization mode
  while (!(CAN1->MSR & 0x1UL));          // Wait for Initialization mode
  printRegister("CAN1->MCR=", CAN1->MCR);

  CAN2->MCR |= 0x1UL;                    // Require CAN2 to Initialization mode
  while (!(CAN2->MSR & 0x1UL));          // Wait for Initialization mode
  printRegister("CAN2->MCR=", CAN2->MCR);

  //CAN1->MCR = 0x51UL;                  // Hardware initialization(No automatic retransmission)
  CAN1->MCR = 0x41UL;                    // Hardware initialization(With automatic retransmission)

  //CAN2->MCR = 0x51UL;                  // Hardware initialization(No automatic retransmission)
  CAN2->MCR = 0x41UL;                    // Hardware initialization(With automatic retransmission)


  // Set bit rates
  CAN1->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF));
  CAN1->BTR |=  (((can_configs[bitrate].TS2 - 1) & 0x07) << 20) | (((can_configs[bitrate].TS1 - 1) & 0x0F) << 16) | ((can_configs[bitrate].BRP - 1) & 0x1FF);
  printRegister("CAN1->BTR=", CAN1->BTR);

  CAN2->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF));
  CAN2->BTR |=  (((can_configs[bitrate].TS2 - 1) & 0x07) << 20) | (((can_configs[bitrate].TS1 - 1) & 0x0F) << 16) | ((can_configs[bitrate].BRP - 1) & 0x1FF);
  printRegister("CAN2->BTR=", CAN2->BTR);

  // Configure Filters to default values
  CAN1->FMR  |=   0x1UL;                 // Set to filter initialization mode
  CAN1->FMR  &= 0xFFFFC0FF;              // Clear CAN2 start bank
  printRegister("CAN1->FMR=", CAN1->FMR);

  // bxCAN has 28 filters.
  // These filters are used for both CAN1 and CAN2.
  // STM32F405 has CAN1 and CAN2, so CAN2 filters are offset by 14
  CAN1->FMR  |= 0xE00;                   // Start bank for the CAN2 interface

  // Set fileter 0
  // Single 32-bit scale configuration
  // Two 32-bit registers of filter bank x are in Identifier Mask mode
  // Filter assigned to FIFO 0
  // Filter bank register to all 0
  CANSetFilter(0, 1, 0, 0, 0x0UL, 0x0UL);

  // Set fileter 14
  // Single 32-bit scale configuration
  // Two 32-bit registers of filter bank x are in Identifier Mask mode
  // Filter assigned to FIFO 0
  // Filter bank register to all 0
  CANSetFilter(14, 1, 0, 0, 0x0UL, 0x0UL);

  CAN1->FMR   &= ~(0x1UL);               // Deactivate initialization mode

  uint16_t TimeoutMilliseconds = 1000;
  bool can2 = false;
  CAN2->MCR   &= ~(0x1UL);               // Require CAN2 to normal mode

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    if ((CAN2->MSR & 0x1UL) == 0) {
      can2 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  //Serial.print("can2=");
  //Serial.println(can2);
  if (can2) {
    Serial.println("CAN2 initialize ok");
  } else {
    Serial.println("CAN2 initialize fail!!");
  }

  bool can1 = false;
  CAN1->MCR   &= ~(0x1UL);               // Require CAN1 to normal mode

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    if ((CAN1->MSR & 0x1UL) == 0) {
      can1 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  //Serial.print("can1=");
  //Serial.println(can1);
  if (can1) {
    Serial.println("CAN1 initialize ok");
  } else {
    Serial.println("CAN1 initialize fail!!");
    return false;
  }
  return true;
}


#define STM32_CAN_TIR_TXRQ              (1U << 0U)  // Bit 0: Transmit Mailbox Request
#define STM32_CAN_RIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_RIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension
#define STM32_CAN_TIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_TIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension

#define CAN_EXT_ID_MASK                 0x1FFFFFFFU
#define CAN_STD_ID_MASK                 0x000007FFU

/**
   Decodes CAN messages from the data registers and populates a
   CAN message struct with the data fields.

   @preconditions     - A valid CAN message is received
   @params CAN_rx_msg - CAN message structure for reception

*/
void CANReceive(uint8_t ch, CAN_msg_t* CAN_rx_msg)
{
  if (ch == 1) {
    uint32_t id = CAN1->sFIFOMailBox[0].RIR;
    if ((id & STM32_CAN_RIR_IDE) == 0) { // Standard frame format
      CAN_rx_msg->format = STANDARD_FORMAT;;
      CAN_rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    }
    else {                               // Extended frame format
      CAN_rx_msg->format = EXTENDED_FORMAT;;
      CAN_rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if ((id & STM32_CAN_RIR_RTR) == 0) {  // Data frame
      CAN_rx_msg->type = DATA_FRAME;
    }
    else {                                // Remote frame
      CAN_rx_msg->type = REMOTE_FRAME;
    }


    CAN_rx_msg->len = (CAN1->sFIFOMailBox[0].RDTR) & 0xFUL;

    CAN_rx_msg->data[0] = 0xFFUL &  CAN1->sFIFOMailBox[0].RDLR;
    CAN_rx_msg->data[1] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 8);
    CAN_rx_msg->data[2] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 16);
    CAN_rx_msg->data[3] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 24);
    CAN_rx_msg->data[4] = 0xFFUL &  CAN1->sFIFOMailBox[0].RDHR;
    CAN_rx_msg->data[5] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 8);
    CAN_rx_msg->data[6] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 16);
    CAN_rx_msg->data[7] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 24);

    CAN1->RF0R |= 0x20UL;
  } // end CAN1

  if (ch == 2) {
    uint32_t id = CAN2->sFIFOMailBox[0].RIR;
    if ((id & STM32_CAN_RIR_IDE) == 0) { // Standard frame format
      CAN_rx_msg->format = STANDARD_FORMAT;;
      CAN_rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    }
    else {                               // Extended frame format
      CAN_rx_msg->format = EXTENDED_FORMAT;;
      CAN_rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if ((id & STM32_CAN_RIR_RTR) == 0) {  // Data frame
      CAN_rx_msg->type = DATA_FRAME;
    }
    else {                                // Remote frame
      CAN_rx_msg->type = REMOTE_FRAME;
    }


    CAN_rx_msg->len = (CAN2->sFIFOMailBox[0].RDTR) & 0xFUL;

    CAN_rx_msg->data[0] = 0xFFUL &  CAN2->sFIFOMailBox[0].RDLR;
    CAN_rx_msg->data[1] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 8);
    CAN_rx_msg->data[2] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 16);
    CAN_rx_msg->data[3] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 24);
    CAN_rx_msg->data[4] = 0xFFUL &  CAN2->sFIFOMailBox[0].RDHR;
    CAN_rx_msg->data[5] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 8);
    CAN_rx_msg->data[6] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 16);
    CAN_rx_msg->data[7] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 24);

    CAN2->RF0R |= 0x20UL;
  } // END CAN2

}

/**
   Encodes CAN messages using the CAN message struct and populates the
   data registers with the sent.

   @params CAN_tx_msg - CAN message structure for transmission

*/
void CANSend(uint8_t ch, CAN_msg_t* CAN_tx_msg)
{
  volatile int count = 0;

  uint32_t out = 0;
  if (CAN_tx_msg->format == EXTENDED_FORMAT) { // Extended frame format
    out = ((CAN_tx_msg->id & CAN_EXT_ID_MASK) << 3U) | STM32_CAN_TIR_IDE;
  }
  else {                                       // Standard frame format
    out = ((CAN_tx_msg->id & CAN_STD_ID_MASK) << 21U);
  }

  // Remote frame
  if (CAN_tx_msg->type == REMOTE_FRAME) {
    out |= STM32_CAN_TIR_RTR;
  }

  if (ch == 1) {
    CAN1->sTxMailBox[0].TDTR &= ~(0xF);
    CAN1->sTxMailBox[0].TDTR |= CAN_tx_msg->len & 0xFUL;

    CAN1->sTxMailBox[0].TDLR  = (((uint32_t) CAN_tx_msg->data[3] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[2] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[1] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[0]      ));
    CAN1->sTxMailBox[0].TDHR  = (((uint32_t) CAN_tx_msg->data[7] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[6] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[5] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[4]      ));

    // Send Go
    CAN1->sTxMailBox[0].TIR = out | STM32_CAN_TIR_TXRQ;

    // Wait until the mailbox is empty
    while (CAN1->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);

    // The mailbox don't becomes empty while loop
    if (CAN1->sTxMailBox[0].TIR & 0x1UL) {
      Serial.println("Send Fail");
      Serial.println(CAN1->ESR);
      Serial.println(CAN1->MSR);
      Serial.println(CAN1->TSR);
    }
  } // end CAN1

  if (ch == 2) {
    CAN2->sTxMailBox[0].TDTR &= ~(0xF);
    CAN2->sTxMailBox[0].TDTR |= CAN_tx_msg->len & 0xFUL;

    CAN2->sTxMailBox[0].TDLR  = (((uint32_t) CAN_tx_msg->data[3] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[2] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[1] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[0]      ));
    CAN2->sTxMailBox[0].TDHR  = (((uint32_t) CAN_tx_msg->data[7] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[6] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[5] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[4]      ));

    // Send Go
    CAN2->sTxMailBox[0].TIR = out | STM32_CAN_TIR_TXRQ;

    // Wait until the mailbox is empty
    while (CAN2->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);

    // The mailbox don't becomes empty while loop
    if (CAN2->sTxMailBox[0].TIR & 0x1UL) {
      Serial.println("Send Fail");
      Serial.println(CAN1->ESR);
      Serial.println(CAN1->MSR);
      Serial.println(CAN1->TSR);
    }
  } // end CAN2

}

/**
  Returns whether there are CAN messages available.

  @returns If pending CAN messages are in the CAN controller

*/
uint8_t CANMsgAvail(uint8_t ch)
{
  if (ch == 1) {
    // Check for pending FIFO 0 messages
    return CAN1->RF0R & 0x3UL;
  } // end CAN1

  if (ch == 2) {
    // Check for pending FIFO 0 messages
    return CAN2->RF0R & 0x3UL;
  } // end CAN2

}


uint8_t counter = 0;
uint8_t frameLength = 0;
unsigned long previousMillis = 0;     // stores last time output was updated
const long interval = 1000;           // transmission interval (milliseconds)

void setup() {

  Serial.begin(115200);
  //bool ret = CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
  //bool ret = CANInit(CAN_50KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
  bool ret = CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
  if (!ret) {
    while (true) {
      Serial.println("Failed init");
      delay(100);
    }
  }

  pixels.begin();
  pixels.clear();

  // Initialize MCP9805 temp. sensor
  Wire.begin();

  // Initialize EEPROM
  Wire_EE.begin();

  // Initialize ICM motion sensor
  if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
    // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {

    Serial.println("Failed to find ICM20649 chip");
    while (1) {
      delay(10);
    }
  }

  pinMode(PC1, OUTPUT);
  pinMode(PC2, OUTPUT);

  digitalWrite(PC1, HIGH);
  digitalWrite(PC2, HIGH);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

  driver.begin();

  //pinMode(CS_PIN, OUTPUT);
  //digitalWrite(CS_PIN, HIGH);


  driver.rms_current(600);
  driver.en_pwm_mode(1);      // Enable extremely quiet stepping
  driver.pwm_autoscale(1);
  driver.microsteps(128);
  driver.toff(1);
  driver.irun(17);
  driver.ihold(10);
  driver.iholddelay(6);
  driver.GLOBAL_SCALER(130);
  //ramp settings
  driver.RAMPMODE(0);
  driver.VSTART(0);
  driver.a1(5);
  driver.v1(40);
  driver.AMAX(1000);
  driver.VMAX(100000);
  driver.DMAX(300);
  driver.d1(40);
  driver.VSTOP(0);
  driver.shaft(shaft);

  //current motor position
  driver.XACTUAL(0);
  hserial2.print("DRV_STATUS=0b");
  hserial2.println(driver.DRV_STATUS(), BIN);

  pinMode(PA10, OUTPUT);
  // Enable write to EEPROM
  digitalWrite(PA10, LOW);

  applicationInit();

}


void loop() {
  CAN_msg_t CAN_TX_msg;
  CAN_msg_t CAN_RX_msg;

  CAN_TX_msg.data[0] = 0x00;
  CAN_TX_msg.data[1] = 0x01;
  CAN_TX_msg.data[2] = 0x02;
  CAN_TX_msg.data[3] = 0x03;
  CAN_TX_msg.data[4] = 0x04;
  CAN_TX_msg.data[5] = 0x05;
  CAN_TX_msg.data[6] = 0x06;
  CAN_TX_msg.data[7] = 0x07;
  CAN_TX_msg.len = frameLength;

  uint8_t send_ch = 1;
  unsigned long currentMillis = millis();
  float tempC;

  uint8_t recv_ch = 1;
  if (CANMsgAvail(recv_ch)) {
    CANReceive(recv_ch, &CAN_RX_msg);
    Serial.print("Channel:");
    Serial.print(recv_ch);

    if (CAN_RX_msg.format == EXTENDED_FORMAT) {
      Serial.print(" Extended ID: 0x");
      if (CAN_RX_msg.id < 0x10000000) Serial.print("0");
      if (CAN_RX_msg.id < 0x1000000) Serial.print("00");
      if (CAN_RX_msg.id < 0x100000) Serial.print("000");
      if (CAN_RX_msg.id < 0x10000) Serial.print("0000");
      Serial.print(CAN_RX_msg.id, HEX);
    } else {
      Serial.print(" Standard ID: 0x");
      if (CAN_RX_msg.id < 0x100) Serial.print("0");
      if (CAN_RX_msg.id < 0x10) Serial.print("00");
      Serial.print(CAN_RX_msg.id, HEX);
      Serial.print("     ");
    }

    Serial.print(" DLC: ");
    Serial.print(CAN_RX_msg.len);
    if (CAN_RX_msg.type == DATA_FRAME) {
      Serial.print(" Data: ");
      for (int i = 0; i < CAN_RX_msg.len; i++) {
        Serial.print("0x");
        Serial.print(CAN_RX_msg.data[i], HEX);
        if (i != (CAN_RX_msg.len - 1))  Serial.print(" ");
      }
      Serial.println();
    } else {
      Serial.println(" Data: REMOTE REQUEST FRAME");
    }
  }

  if (CAN_RX_msg.data[0] == 0x00)
  {
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.show();

    Serial.println("Record data");

    //Serial.print("Temperature C: ");
    tempC = getTempC_MCP9805();
    //Serial.println(tempC);

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp);

    /* Display the results (acceleration is measured in m/s^2) */
    /*Serial.print("\t\tAccel X: ");
      Serial.print(accel.acceleration.x);
      Serial.print(" \tY: ");
      Serial.print(accel.acceleration.y);
      Serial.print(" \tZ: ");
      Serial.print(accel.acceleration.z);
      Serial.println(" m/s^2 ");*/

    /*Display the results (acceleration is measured in m/s^2) */
    /*Serial.print("\t\tGyro X: ");
      Serial.print(gyro.gyro.x);
      Serial.print(" \tY: ");
      Serial.print(gyro.gyro.y);
      Serial.print(" \tZ: ");
      Serial.print(gyro.gyro.z);
      Serial.println(" radians/s ");
      Serial.println();*/


    uint8_t status;
    status = angle4_getNewAngle(&Angle);
    /*Serial.print("_ANGLE4_REG_CUSTOMER_CFG_1: ");
      Serial.println(angle4_readByte(_ANGLE4_REG_CUSTOMER_CFG_1));*/

    /*Serial.print("status = ");
      Serial.println(status);
      Serial.print("angle = ");
      Serial.println(Angle);*/

    /*Serial.print("Address "); Serial.print(mem_addr); Serial.print(" before writing: ");
      Serial.println(readEEAddr(mem_addr));

      writeEEAddr(mem_addr, round(tempC));

      Serial.print("Address "); Serial.print(mem_addr); Serial.print(" after writing: ");
      Serial.println(readEEAddr(mem_addr));

      mem_addr++;*/

    writeEEAddr(1, round(tempC));
    Serial.print("Temperature "); Serial.print(tempC); Serial.println(" stored to address 1");

    writeEEAddr(2, round(accel.acceleration.x));
    Serial.print("AccX "); Serial.print(accel.acceleration.x); Serial.println(" stored to address 2");

    writeEEAddr(3, round(accel.acceleration.y));
    Serial.print("AccY "); Serial.print(accel.acceleration.y); Serial.println(" stored to address 3");

    writeEEAddr(4, round(accel.acceleration.z));
    Serial.print("AccZ "); Serial.print(accel.acceleration.z); Serial.println(" stored to address 4");

    writeEEAddr(5, round(gyro.gyro.x));
    Serial.print("GyroX "); Serial.print(gyro.gyro.x); Serial.println(" stored to address 5");

    writeEEAddr(6, round(gyro.gyro.y));
    Serial.print("GyroY "); Serial.print(gyro.gyro.y); Serial.println(" stored to address 6");

    writeEEAddr(7, round(gyro.gyro.z));
    Serial.print("GyroZ "); Serial.print(gyro.gyro.z); Serial.println(" stored to address 7");

    writeEEAddr(8, round(gyro.gyro.z));
    Serial.print("GyroZ "); Serial.print(gyro.gyro.z); Serial.println(" stored to address 8");

    writeEEAddr(9, round(Angle));
    Serial.print("Angle "); Serial.print(Angle); Serial.println(" stored to address 9");
  }

  else if (CAN_RX_msg.data[0] == 0x01)
  {
    driver.shaft(true);
    pixels.setPixelColor(0, pixels.Color(0, 0, 150));
    pixels.show();
    Serial.println("Get data");

    writeEEAddr(1, round(tempC));
    Serial.print("Temperature "); Serial.print(tempC); Serial.println(" stored to address 1");

    writeEEAddr(2, round(accel.acceleration.x));
    Serial.print("AccX "); Serial.print(accel.acceleration.x); Serial.println(" stored to address 2");

    writeEEAddr(3, round(accel.acceleration.y));
    Serial.print("AccY "); Serial.print(accel.acceleration.y); Serial.println(" stored to address 3");

    writeEEAddr(4, round(accel.acceleration.z));
    Serial.print("AccZ "); Serial.print(accel.acceleration.z); Serial.println(" stored to address 4");

    writeEEAddr(5, round(gyro.gyro.x));
    Serial.print("GyroX "); Serial.print(gyro.gyro.x); Serial.println(" stored to address 5");

    writeEEAddr(6, round(gyro.gyro.y));
    Serial.print("GyroY "); Serial.print(gyro.gyro.y); Serial.println(" stored to address 6");

    writeEEAddr(7, round(gyro.gyro.z));
    Serial.print("GyroZ "); Serial.print(gyro.gyro.z); Serial.println(" stored to address 7");

    writeEEAddr(8, round(gyro.gyro.z));
    Serial.print("GyroZ "); Serial.print(gyro.gyro.z); Serial.println(" stored to address 8");

    writeEEAddr(9, round(Angle));
    Serial.print("Angle "); Serial.print(Angle); Serial.println(" stored to address 9");
  }
  else {
    Serial.println("Not a valid data");
  }


  delay(2000);

}

void writeEEAddr(uint16_t addr, uint8_t val)
{
  Wire_EE.beginTransmission(EEPROM_Addr);

  Wire_EE.write((addr) >> 8);
  Wire_EE.write((addr) & (0xFF));

  Wire_EE.write(val);

  Serial.println("Value: ");
  Serial.println(val);

  Serial.println(Wire_EE.endTransmission(true));

  delay(10);
}

byte readEEAddr(uint16_t addr)
{
  Wire_EE.beginTransmission(EEPROM_Addr);

  Wire_EE.write((addr) >> 8);
  Wire_EE.write((addr) & (0xFF));

  Serial.println(Wire_EE.endTransmission(false));

  delay(10);

  Wire_EE.requestFrom(EEPROM_Addr, 1);

  if (Wire_EE.available())
  {
    return (Wire_EE.read());
  }
  return 0x00;
}

void init_MCP9805()
{
  // Start I2C Transmission
  Wire.beginTransmission(MCP9805_Addr);
  // Select configuration register
  Wire.write(0x01);
  // Continuous conversion mode, Power-up default
  Wire.write(0x00);
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(MCP9805_Addr);
  // Select resolution rgister
  Wire.write(0x08);
  // Resolution = +0.0625 / C
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();
}

int getTemp_MCP9805()
{
  unsigned int data[2];

  // Starts I2C communication
  Wire.beginTransmission(MCP9805_Addr);
  // Select data register
  Wire.write(0x05);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(MCP9805_Addr, 2);

  // Read 2 bytes of data
  // temp MSB, temp LSB
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to 13-bits
  int temp = ((data[0] & 0x1F) * 256 + data[1]);
  if (temp > 4095)
  {
    temp -= 8192;
  }

  return temp;
}

float getTempC_MCP9805()
{
  int temp = getTemp_MCP9805();

  return (temp * 0.0625);
}

float getTempF_MCP9805()
{
  int tempC = getTempC_MCP9805();

  return (tempC * 1.8 + 32);
}

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

  pinMode(PB12, OUTPUT);
  //  mySPI.setDataMode(SPI_MODE3);
  //  mySPI.setClockDivider(SPI_CLOCK_DIV128);
  //  mySPI.setBitOrder(MSBFIRST);
  mySPI.beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE3));

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

void TMC2130Stepper::switchCSpin(bool state) {
  delayMicroseconds(100);
  digitalWrite(_pinCS, state);
  delayMicroseconds(100);
}
