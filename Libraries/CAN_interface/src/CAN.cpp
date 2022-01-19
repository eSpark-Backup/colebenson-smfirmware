#include "CAN.h"

extern "C"
{
	
	/*********************************************************************************
	 *	@purpose: initializes CAN
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	uint8_t initCAN()
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: transmit byte
	 *	
	 *  @byte: byte to be transmitted
	 *  @msg_id: mesage id
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t transmitByte(uint8_t byte, uint32_t msg_id, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: transmit word
	 *	
	 *  @word: word to be transmitted
	 *  @msg_id: mesage id
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t transmitWord(uint16_t word, uint32_t msg_id, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: transmit double word
	 *	
	 *  @dword: double word to be transmitted
	 *  @msg_id: mesage id
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t transmitDWord(uint32_t dword, uint32_t msg_id, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: transmit CAN_msg_t struct
	 *	
	 *  @DS: word to be transmitted
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t transmitDS(CAN_msg_t DS)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: receive byte
	 *	
	 *  @byte: reference of byte to store at
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t receiveByte(uint8_t* byte, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: receive word
	 *	
	 *  @word: reference of word to store at
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t receiveWord(uint16_t* word, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: receive double word
	 *	
	 *  @dword: reference of dword to store at
	 *	@ch: channel id
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t receiveDWORD(uint32_t* dword, uint8_t ch)
	{
		return FAILED;
	}
	
	/*********************************************************************************
	 *	@purpose: receive CAN_msg_t struct
	 *	
	 *  @DS: reference of CAN_msg_t struct to store at
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	
	uint8_t receiveDS(CAN_msg_t* DS)
	{
		return FAILED;
	}
}