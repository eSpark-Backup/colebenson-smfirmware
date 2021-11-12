#include <SPI.h>

int chipCS = PA4;
const byte STEPOUT = PC7;
int enable = PC11;

#define EN_PIN           PC11 // Enable
#define DIR_PIN          PC8  // Direction
#define STEP_PIN         PC7  // Step
#define CS_PIN           PA4 // Chip select
#define SW_MOSI          PA7 // Software Master Out Slave In (MOSI)
#define SW_MISO          PA6 // Software Master In Slave Out (MISO)
#define SW_SCK           PA5 // Software Slave Clock (SCK)
#define SW_RX            PA1 // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX            PA0 // TMC2208/TMC2224 SoftwareSerial transmit pin

SPIClass mySPI(SW_MOSI, SW_MISO, SW_SCK);

#define SPI mySPI

void setup() {
  pinMode(chipCS, OUTPUT);
  pinMode(STEPOUT, OUTPUT);
  pinMode(enable, OUTPUT);
  digitalWrite(chipCS, HIGH);
  digitalWrite(enable, HIGH);
  digitalWrite(STEPOUT, LOW);

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  // init motor
  // ---------------------
  SPISendData(0x80, 0x00000080);     // GCONF
  SPISendData(0xED, 0x00000000);     // SGT
  SPISendData(0x94, 0x00000040);     // TCOOLTHRS
  SPISendData(0x89, 0x00010606);     // SHORTCONF
  SPISendData(0x90, 0x00080303);     // IHOLD_IRUN
  SPISendData(0xEC, 0x15410153);     // CHOPCONF - microstepping 8
  SPISendData(0xF0, 0xC40C001E);     // PWMCONF

  // ---------------------
  digitalWrite(enable, LOW);
}

void loop() {
  for (uint16_t i = 5000; i > 0; i--) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(160);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(160);
  }
}

void SPISendData(unsigned long address, unsigned long datagram)
{
  //TMC5130 takes 40 bit data: 8 address and 32 data
  digitalWrite(chipCS, LOW);
  delayMicroseconds(10);

  SPI.transfer(address);
  SPI.transfer((datagram >> 24) & 0xff);
  SPI.transfer((datagram >> 16) & 0xff);
  SPI.transfer((datagram >> 8) & 0xff);
  SPI.transfer((datagram) & 0xff);
  digitalWrite(chipCS, HIGH);
}
