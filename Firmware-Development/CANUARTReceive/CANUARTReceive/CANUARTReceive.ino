// RECV EXAMPLE OF SERIAL CAN MODULE
// unsigned char recv(unsigned long *id, uchar *buf);
// SUPPORT: joney.sui@longan-labs.cc
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>

Serial_CAN can;


SoftwareSerial mySerial(PB_6, PB_7); // RX, TX

#define Serial mySerial

#define can_tx  PB_8           // tx of serial can module connect to PB8
#define can_rx  PB_9           // rx of serial can module connect to PB9

void setup()
{
    mySerial.begin(115200);
    can.begin(can_tx, can_rx, 115200);      // tx, rx
    //can.factorySetting();
    mySerial.println("begin");
}

unsigned long id = 0;
unsigned char dta[8];

// send(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf);
void loop()
{
  
    if(can.recv(&id, dta))
    {
        Serial.print("GET DATA FROM ID: ");
        Serial.println(id);
        for(int i=0; i<8; i++)
        {
            Serial.print("0x");
            Serial.print(dta[i], HEX);
            Serial.print('\t');
        }
        Serial.println();
    }

}

// END FILE
