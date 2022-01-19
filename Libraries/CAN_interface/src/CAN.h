#pragma once

#include "Arduino.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

#define STM32_CAN_TIR_TXRQ              (1U << 0U)  // Bit 0: Transmit Mailbox Request
#define STM32_CAN_RIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_RIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension
#define STM32_CAN_TIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_TIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension

#define CAN_EXT_ID_MASK                 0x1FFFFFFFU
#define CAN_STD_ID_MASK                 0x000007FFU


// variables and structs
struct CAN_msg_t
{
  uint32_t id;        /* 29 bit identifier                               */
  uint8_t  data[8];   /* Data field                                      */
  uint8_t  len;       /* Length of data field in bytes                   */
  uint8_t  ch;        /* Object channel(Not use)                         */
  uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
};

typedef const struct
{
  uint8_t TS2;
  uint8_t TS1;
  uint8_t BRP;
} CAN_bit_timing_config_t;


/* Symbolic names for bit rate of CAN message                                */
typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

CAN_bit_timing_config_t can_configs[6] = {{2, 12, 56}, {2, 12, 28}, {2, 13, 21}, {2, 11, 12}, {2, 11, 6}, {1, 5, 6}};


extern "C"
{
	// Public functions
	uint8_t initCAN();
	uint8_t transmitByte(uint8_t byte, uint32_t msg_id, uint8_t ch);
	uint8_t transmitWord(uint16_t word, uint32_t msg_id, uint8_t ch);
	uint8_t transmitDWord(uint32_t dword, uint32_t msg_id, uint8_t ch);
	uint8_t transmitDS(CAN_msg_t* DS);
	uint8_t receiveByte(uint8_t* byte, uint8_t ch);
	uint8_t receiveWord(uint16_t* word, uint8_t ch);
	uint8_t receiveDword(uint32_t* dword, uint8_t ch);
	uint8_t receiveDS(CAN_msg_t* DS);
	
	// Private functions
	static uint8_t CANInit(BITRATE bitrate, int remap);
	static void CANSetGpio(GPIO_TypeDef * addr, uint8_t index, uint8_t speed = 3);
	static void CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2);
	static void printRegister(char * buf, uint32_t reg);
	static void CANSend(uint8_t ch, CAN_msg_t* CAN_tx_msg);
	static uint8_t CANMsgAvail(uint8_t ch);
	static void CANReceive(uint8_t ch, CAN_msg_t* CAN_rx_msg);
}