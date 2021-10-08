// debug mode
#include <SoftwareSerial.h>
SoftwareSerial mySerial(PA_1, PA_0); // RX, TX
#include <Serial_CAN_Module.h>

Serial_CAN can;
SoftwareSerial *canSerial2 = NULL;
#define Serial mySerial
//SoftwareSerial mySerial(PA_1, PA_0); // RX, TX


#define can_tx  PB_8           // tx of serial can module connect to PB8
#define can_rx  PB_9           // rx of serial can module connect to PB9

void setup()
{
  mySerial.begin(115200);
  //can.baudRate(0);
  can.begin(can_tx, can_rx, 115200);      // tx, rx
  /*canSerial2 = new SoftwareSerial(can_tx, can_rx);
   * 
   * 
  canSerial2->begin(115200);*/
  pinMode(PD_13, OUTPUT);
  pinMode(PD_12, OUTPUT);
  mySerial.println("begin");
}

void loop()
{
  /*Serial.println("Test");
    digitalWrite(PD_13, HIGH);
    delay(500);
    digitalWrite(PD_13, LOW);
    delay(500);*/

   mySerial.println("Test");
   //mySerial.println(can.cmdOk("+++"));

   delay(1000);

  //can.debugMode();

  /*while (canSerial2->available())
  {
    digitalWrite(PD_13, HIGH);
    digitalWrite(PD_12, LOW);
    Serial.write(canSerial2->read());
  }*/

  /*canSerial2->write("Hello");
    delay(1000);*/


}

// END FILE
