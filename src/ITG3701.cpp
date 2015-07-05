/*
    ITG3701.cpp
    C. Huffine (ODU)

    Driver for the ITG3701 enhanced gyroscope

    This is a library for the Invensense ITG3701 evaluation board
    ----> https://store.invensense.com/ProductDetail/EMSENSRITG3701-Embedded-Masters/519165/

    This code is heavily based upon OneHorse's work with his demo code (https://github.com/kriswiner/ITG3701_LSM303D_MS5637/blob/master/ITG3701_LSM303D_MS5637.ino)
    As well as the Sparkfun I2C abstraction and library format. I appreciate their work greatly.

    Version 1.0
*/

#include "Arduino.h"

#include <Wire.h>

#include "ITG3701.h"

//  Instantiates a new ITG3701 class w/appropriate properties

ITG3701::ITG3701(uint8_t i2cAddress)
{
   m_i2cAddress = i2cAddress;
}

//  Sets up the Wire interface

void ITG3701::begin(Gscale m_gscale, Godr m_godr, Gbw m_gbw)
{
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_PWR_MGMT_1, 0x00);  // Clear sleep mode bit (6), enable all sensors
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_CONFIG, 0x03);
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_SMPLRT_DIV, 0x04);
  uint8_t c = I2CreadByte(ITG3701_ADDRESS, ITG3701_GYRO_CONFIG);
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_GYRO_CONFIG, c & ~0x02); // Clear Fchoice bits [1:0]
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_GYRO_CONFIG, c | m_gscale << 3); // Set full scale range for the gyro
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_INT_PIN_CFG, 0x20);
  I2CwriteByte(ITG3701_ADDRESS, ITG3701_INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
  Wire.begin();
}

void ITG3701::updateGyro(){
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  I2CreadBytes(ITG3701_ADDRESS, ITG3701_GYRO_XOUT_H, &rawData[0], 6);  // Read the six raw data registers sequentially into data array
  x = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
  y = ((int16_t)rawData[2] << 8) | rawData[3] ;
  z = ((int16_t)rawData[4] << 8) | rawData[5] ;
}

// Wire.h read and write protocols
void ITG3701::I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

uint8_t ITG3701::I2CreadByte(uint8_t address, uint8_t subAddress)
{
	uint8_t data; // `data` will store the register data
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

void ITG3701::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	// Next send the register to be read. OR with 0x80 to indicate multi-read.
	Wire.write(subAddress | 0x80);     // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
	Wire.requestFrom(address, count);  // Read bytes from slave register address
	while (Wire.available())
	{
		dest[i++] = Wire.read(); // Put read results in the Rx buffer
	}
}
