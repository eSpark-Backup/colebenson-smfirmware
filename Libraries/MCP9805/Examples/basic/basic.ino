#include "MCP9805.h"

float tempC;
float tempF;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  initMCP();

}

void loop() {
  // put your main code here, to run repeatedly:
  getTempC(&tempC);
  getTempF(&tempF);

  Serial.println("Temperature in C");
  Serial.println(tempC);

  Serial.println("Temperature in F");
  Serial.println(tempF);

  delay(1000);

}
