#include "antenna_utils.h"

String direction = "";
bool useDRDYPin = false; // Assuming default as false, but can be changed in `setup()`
bool measurementReadingMode = true; // default mode set to continuous
uint16_t cycleCount = 200; // Default cycle count
float gain = 75.0;  // Default gain
bool hasMoved = false;  // Flag to indicate if the motor has moved

// Read a value from the specified register
uint8_t readReg(uint8_t reg)
{
  Wire.beginTransmission(RM3100Address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(RM3100Address, 1);
  return Wire.read();
}

// Write a value to a specified register
void writeReg(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(RM3100Address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Change the cycle count for the sensor
void changeCycleCount(uint16_t newCC)
{
  uint8_t CCMSB = (newCC & 0xFF00) >> 8; // Most significant byte
  uint8_t CCLSB = newCC & 0xFF;          // Least significant byte

  Wire.beginTransmission(RM3100Address);
  Wire.write(RM3100_CCX1_REG);
  Wire.write(CCMSB);
  Wire.write(CCLSB);
  Wire.endTransmission();
}

// Function to read data from the compass sensor
float readFromCompass()
{
  uint32_t x = 0, y = 0, z = 0;
  uint32_t x2, x1, x0, y2, y1, y0, z2, z1, z0;

  if (useDRDYPin)
  {
    while (digitalRead(PIN_DRDY) == LOW)
      ;
  }
  else
  {
    while ((readReg(RM3100_STATUS_REG) & 0x80) != 0x80)
      ;
  }

  Wire.beginTransmission(RM3100Address);
  Wire.write(RM3100_X_AXIS_REG_READ);
  Wire.endTransmission();
  Wire.requestFrom(RM3100Address, 9);
  
  if (Wire.available() == 9)
  {
    x2 = Wire.read();
    x1 = Wire.read();
    x0 = Wire.read();
    y2 = Wire.read();
    y1 = Wire.read();
    y0 = Wire.read();
    z2 = Wire.read();
    z1 = Wire.read();
    z0 = Wire.read();
  }

  x = (x * 256 * 256 * 256) | (int32_t)(x2) * 256 * 256 | (uint16_t)(x1) * 256 | x0;
  y = (y * 256 * 256 * 256) | (int32_t)(y2) * 256 * 256 | (uint16_t)(y1) * 256 | y0;
  z = (z * 256 * 256 * 256) | (int32_t)(z2) * 256 * 256 | (uint16_t)(z1) * 256 | z0;

  double uT = sqrt(pow(((float)(x) / gain), 2) + pow(((float)(y) / gain), 2) + pow(((float)(z) / gain), 2));
  float yxHeading = atan2(y, x);
  float zxHeading = atan2(z, x);

  float currentAngle = yxHeading * 180 / PI + DECLINATION_ANGLE;
  if (currentAngle < 0) currentAngle += 360;

  return currentAngle;
}

// Function to read the telegram from the BKG
float receiveTelegramFromBKG()
{
  uint8_t DATA_PACKET[PROTOCOL_SIZE] = {0};
  uint16_t targetAngle = 0;

  if (Serial.available() > 0)
  {
    for (int i = 0; i < PROTOCOL_SIZE; i++)
    {
      DATA_PACKET[i] = Serial.read();
      delay(100);
    }
    targetAngle = (DATA_PACKET[8] << 8) | DATA_PACKET[9];
    return (float)targetAngle * BIN_TO_DEG_COEFF;
  }
  else
  {
    Serial.println("Serial read error.");
    return 0;
  }
}

