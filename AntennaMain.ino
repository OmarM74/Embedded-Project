#include "antenna_utils.h"

void setup()
{
  pinMode(PIN_DRDY, INPUT);
  pinMode(PIN_SCL, INPUT_PULLUP);
  pinMode(PIN_SDA, INPUT_PULLUP);

  Wire.begin();
  Serial.begin(9600);
  delay(100);

  // Initialize settings and calibration
  revid = readReg(RM3100_REVID_REG);
  Serial.print("REVID ID = 0x");
  Serial.println(revid, HEX);
  
  changeCycleCount(200);  // Initial cycle count
  gain = 75.0;  // Default gain

  if (measurementReadingMode)
  {
    writeReg(RM3100_CMM_REG, 0x79);
  }
  else
  {
    writeReg(RM3100_CMM_REG, 0x00);
    writeReg(RM3100_POLL_REG, 0x70);
  }

  // Motor Setup
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);

  angleStepper.setSpeed(60);  // Set speed of the motor
}

void loop()
{
  float currentAngle = readFromCompass();
  Serial.print("Compass Values (Degrees): ");
  Serial.println(currentAngle);

  float targetAngle = receiveTelegramFromBKG();
  float angleDifference = abs(targetAngle - currentAngle);

  if (angleDifference > 10)
  {
    if (targetAngle > currentAngle)
    {
      angleStepper.step(1);
    }
    else
    {
      angleStepper.step(-1);
    }
  }
  else if (angleDifference <= 1)
  {
    angleStepper.step(0);  // Stop motor
  }

  delay(1000);
}

