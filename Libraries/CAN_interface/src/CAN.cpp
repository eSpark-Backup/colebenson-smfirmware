#include "CAN.h"

// Constants and macros
#define DEBUG 0

extern "C"
{
	
	/*********************************************************************************
	 *	@purpose: initializes CAN
	 *	
	 *	@return: status, SUCCESS or FAILED
	 *********************************************************************************/
	 
	uint8_t initCAN()
	{
		uint8_t ret = CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
		return ret ? SUCCESS: FAILED;
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
	
	/*********************************************************************************
	   @purpose: Print registers.
	 *********************************************************************************/
	static void printRegister(char * buf, uint32_t reg) {
	  if (DEBUG == 0) return;
	  Serial.print(buf);
	  Serial.print(reg, HEX);
	  Serial.println();
	}
	
	/*********************************************************************************
	 *	@purpose: initializes the CAN controller with specified bit rate
	 *
	 *  @bitrate: specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
	 *  @remap: Select CAN port.
	 *					  =0:CAN1_RX mapped to PA11, CAN1_TX mapped to PA12
	 *						 CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6
	 *					  =1:Not used
	 *					  =2:CAN1_RX mapped to PB8,  CAN1_TX mapped to PB9 (not available on 36-pin package)
	 *						 CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
	 *					  =3:CAN1_RX mapped to PD0,  CAN1_TX mapped to PD1 (available on 100-pin and 144-pin package)
	 *						 CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
	 *********************************************************************************/
	static uint8_t CANInit(BITRATE bitrate, int remap)
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
	
	/*********************************************************************************
	 *	@purpose: initializes the CAN GPIO registers.
	 *  
	 *	@addr: Specified GPIO register address.
	 *	@index: Specified GPIO index.
	 *	@speed: Specified OSPEEDR register value.(Optional)
	 *********************************************************************************/
	 
	static void CANSetGpio(GPIO_TypeDef * addr, uint8_t index, uint8_t speed) {
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
	
	/*********************************************************************************
	 *	@purpose: initializes the CAN filter registers.
	 *  
	 *	@preconditions   This register can be written only when the filter initialization mode is set (FINIT=1) in the CAN_FMR register.
	 *	@index:  pecified filter index. index 27:14 are available in connectivity line devices only.
	 *	@scale: Select filter scale.
	 *					  0: Dual 16-bit scale configuration
	 *					  1: Single 32-bit scale configuration
	 *	@mode:  Select filter mode.
	 *					  0: Two 32-bit registers of filter bank x are in Identifier Mask mode
	 *					  1: Two 32-bit registers of filter bank x are in Identifier List mode
	 *	@fifo: Select filter assigned.
	 *						0: Filter assigned to FIFO 0
	 *					  1: Filter assigned to FIFO 1
	 *	@bank1: Filter bank register 1
     *	@bank2: Filter bank register 2
	 *********************************************************************************/

	static void CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2) {
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
}