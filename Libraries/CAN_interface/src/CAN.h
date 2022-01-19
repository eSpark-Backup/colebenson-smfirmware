#pragma once

#include "Arduino.h"

// Constant macros
#define FAILED 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

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

extern "C"
{
	// Public functions
	uint8_t initCAN();
	uint8_t transmitByte(uint8_t byte, uint32_t msg_id, uint8_t ch);
	uint8_t transmitWord(uint16_t word, uint32_t msg_id, uint8_t ch);
	uint8_t transmitDWord(uint32_t dword, uint32_t msg_id, uint8_t ch);
	uint8_t transmitDS(CAN_msg_t DS);
	uint8_t receiveByte(uint8_t* byte, uint8_t ch);
	uint8_t receiveWord(uint16_t* word, uint8_t ch);
	uint8_t receiveDWORD(uint32_t* dword, uint8_t ch);
	uint8_t receiveDS(CAN_msg_t* DS);
	
	// Private functions
	
}