#include <Arduino.h>
#include <TMCStepper.h>


#define EN_PIN           PC11 // Enable
#define DIR_PIN          PC8  // Direction
#define STEP_PIN         PC7  // Step
#define CS_PIN           PA4
#define MOSI_PIN         PA7
#define MISO_PIN         PA6
#define SCK_PIN          PA5
#define R_SENSE          0.0165f

constexpr uint16_t
  en_pin = EN_PIN,
  step_pin = STEP_PIN,
  dir_pin = DIR_PIN,
  cs_pin = CS_PIN;

//TMC5130Stepper driver(cs_pin, 0.11);

TMC5160Stepper driver(cs_pin, R_SENSE, MOSI_PIN, MISO_PIN, SCK_PIN);

void abort(const String &msg) {
  Serial.print("ERROR: ");
  Serial.println(msg);
  while(true);
}

void setup() {

  Serial.begin(115200);
  while(!Serial);

  Serial.println(F(
    "\nTMCStepper TMC internal motion controller example.\n"
    "Compiled " __DATE__ " " __TIME__ "\n"
    "Start..."
    ));
  
  pinMode(en_pin, OUTPUT);
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);
  digitalWrite(en_pin, HIGH);

  //SPI.begin();
  driver.begin();

  TMC5130_n::IOIN_t ioin{ driver.IOIN() };

  if (ioin.version == 0 || ioin.version == 0xFF) {
    abort("Communication error");
  }
  else {
    Serial.print("Driver version: ");
    Serial.println(ioin.version);
  }

  if (ioin.sd_mode) {
    Serial.println("Driver configured for step and dir interface");
  }
  else {
    abort("Driver configured for internal motion controller interface");
  }

  // Test en pin
  if (!driver.drv_enn()) abort("Was not able to assert en pin");

  // Test step pin
  digitalWrite(step_pin, HIGH);
  if (!driver.refl_step()) abort("Was not able to assert step pin");

  digitalWrite(step_pin, LOW);
  if ( driver.refl_step()) abort("Was not able to assert step pin");

  // Test dir pin
  digitalWrite(dir_pin, HIGH);
  if (!driver.refr_dir()) abort("Was not able to assert dir pin");

  digitalWrite(dir_pin, LOW);
  if ( driver.refr_dir()) abort("Was not able to assert dir pin");

  driver.toff(3);
  driver.rms_current(600);
  driver.en_pwm_mode(true);

  digitalWrite(en_pin, LOW);
  if (driver.drv_enn()) abort("Was not able to assert en pin");
}

void loop() {
  digitalWrite(step_pin, HIGH);
  delayMicroseconds(200);
  digitalWrite(step_pin, LOW);
  delayMicroseconds(200);
}
