#include "ICM.h"

sensor_t gyro;
sensor_t acceleration;

void setup() {
  // put your setup code here, to run once:
  initICM();
}

void loop() {
  // put your main code here, to run repeatedly:
  getGyro(&gyro);
  getAccel(&acceleration);

  Serial.println("Gyro X");
  Serial.println(gyro.x);

  Serial.println("Gyro Y");
  Serial.println(gyro.y);

  Serial.println("Gyro Z");
  Serial.println(gyro.z);

  Serial.println();

  Serial.println("Acc X");
  Serial.println(acceleration.x);

  Serial.println("Acc Y");
  Serial.println(acceleration.y);

  Serial.println("Acc Z");
  Serial.println(acceleration.z);

  Serial.println();

  delay(1000);

}
