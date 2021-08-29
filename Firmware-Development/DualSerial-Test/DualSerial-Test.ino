#include <SoftwareSerial.h>

const int led  = 60;
const int led2 = 61;
char a = '0';
char b = '0';

SoftwareSerial mySerial(3, 2); // RX, TX
SoftwareSerial mySerial2(27, 26); // RX, TX

void setup() {
  mySerial.begin(9600);
  mySerial2.begin(9600);
  pinMode(led,  OUTPUT);
  pinMode(led2, OUTPUT);
  delay (100);
}

void loop() {
  mySerial2.listen();
  
  while (mySerial2.available() > 0) {
    b = mySerial2.read();
    mySerial.write(b);
  }

  mySerial.listen();
  
  while (mySerial.available() > 0) {
    a = mySerial.read();
    mySerial2.write(a);
  }

  //Serial2.println("ABC");


//  mySerial2.println(b);
//  mySerial.println(a);
//  mySerial.println("Test");
//  delay(500);
//  digitalWrite(led,  HIGH);
//  digitalWrite(led2, HIGH);
//  delay(500);
//  digitalWrite(led,  LOW);
//  digitalWrite(led2, LOW);
  delay(500);
}
