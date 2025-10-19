#ifndef ANTENNA_UTILS_H
#define ANTENNA_UTILS_H

#include <Wire.h>

// Define Pins
#define PIN_DRDY 2  // Data Ready Pin
#define PIN_SCL 3   // I2C Clock Pin
#define PIN_SDA 4   // I2C Data Pin

// RM3100 register addresses
#define RM3100_REVID_REG 0x00
#define RM3100_CMM_REG 0x01
#define RM3100_CCX1_REG 0x02
#define RM3100_CCX0_REG 0x03
#define RM3100_STATUS_REG 0x04
#define RM3100_X_AXIS_REG_READ 0x10
#define RM3100_POLL_REG 0x20

// Constants
#define PROTOCOL_SIZE 25
#define BIN_TO_DEG_COEFF 0.1
#define DECLINATION_ANGLE 0.0  // Adjust according to your location

extern String direction;  // Current direction of the antenna
extern bool useDRDYPin;
extern bool measurementReadingMode;
extern uint16_t cycleCount;
extern float gain;
extern bool hasMoved;

// Function declarations
void setup();
void loop();
float readFromCompass();
void changeCycleCount(uint16_t newCC);
float receiveTelegramFromBKG();
void writeReg(uint8_t reg, uint8_t value);
uint8_t readReg(uint8_t reg);
void serialPrintArray(uint8_t* array, uint16_t size);

#endif

