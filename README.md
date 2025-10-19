Overview:

This project is designed to control the direction of a drone's antenna system using a stepper motor, guided by the data received from an RM3100 magnetometer (compass). The system reads the current angle (Istwinkel) of the antenna and adjusts it towards the desired angle (Sollwinkel) based on serial data received from the drone. This ensures that the antenna is always oriented correctly in relation to the drone's desired direction.

Features:

Magnetometer Integration: Utilizes the RM3100 magnetometer to read the current orientation of the antenna.

Antenna Control: The system adjusts the antenna’s direction using a stepper motor, based on calculated heading differences.

Real-time Adjustment: Continuously adjusts the antenna’s angle to align with the desired heading from the drone.

Serial Communication: Receives control data from the drone via serial communication to update the antenna’s position.

Hardware Requirements:

ATmega328P Microcontroller from Microchip Technology (formerly Atmel)

RM3100 Magnetometer: A 3-axis magnetometer for compass readings.

Stepper Motor: Controls the movement of the antenna based on angle changes.

Motor Driver: Drives the stepper motor.

Wiring and Pin Connections: Correct pin setup for I2C communication, motor control, and sensor connections.

Setup:

Hardware Setup:

Connect the RM3100 magnetometer to the ATmega328P microcontroller via I2C (SCL, SDA pins).

Connect the stepper motor to the motor driver and then connect the driver to the ATmega328P pins for control.

Ensure that the Data Ready Pin (DRDY) of the RM3100 is connected to an appropriate digital input pin on the ATmega328P.

Software Setup:

Upload the main.cpp to your microcontroller using your preferred IDE (e.g., Atmel Studio or PlatformIO).

Include the necessary libraries for I2C communication (e.g., Wire).

Set up the serial monitor to view debugging information and compass data.

File Breakdown:

antenna_utils.h: Header file that defines constants, function prototypes, and configuration settings for the system.

antenna_utils.c: C file containing the function implementations for reading sensor data and controlling the motor.

main.cpp: The main program that initializes the system and continuously adjusts the antenna's position based on incoming data.

Key Functions:

readFromCompass(): Reads the current heading (direction) from the RM3100 magnetometer.

changeCycleCount(): Modifies the cycle count to change the data acquisition rate of the RM3100 sensor.

receiveTelegramFromBKG(): Reads the incoming serial data and extracts the desired angle for the antenna.

antennaControl(): Adjusts the antenna’s position based on the calculated angle difference between the current and desired heading.

How It Works:

Initialization: The program begins by initializing the RM3100 magnetometer, configuring the system for continuous data collection, and calibrating the antenna to point north.

Compass Reading: The system reads the current angle of the antenna using the magnetometer, converting the raw sensor data into degrees.

Data Reception: The ATmega328P listens for incoming serial data from the drone, which includes the desired antenna angle.

Motor Adjustment: The program calculates the difference between the current and desired heading, then adjusts the stepper motor accordingly to align the antenna with the desired position.

Calibration:

At startup, the system automatically calibrates the antenna to point north using the compass readings from the RM3100. This initial calibration ensures that the system has a reference point before adjusting the antenna’s angle.

Once calibration is complete, the system will continuously update the antenna’s position based on the serial data received from the drone, keeping it aligned with the desired heading.

Usage:

Search Mode: If the system detects that the antenna is not aligned with the desired angle, it will incrementally move the antenna by predefined steps, adjusting until the correct heading is achieved.

Automatic Mode: After initialization, the system automatically adjusts the antenna’s angle based on the desired heading sent by the drone.

Limitations:

The system assumes that the stepper motor has no obstructions and can freely move the antenna.

The magnetometer’s readings can be influenced by nearby magnetic fields, which may cause inaccuracies.

The accuracy of the heading may be affected by the declination angle, which needs to be manually adjusted.
