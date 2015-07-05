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

////////////////////////////
// ITG3701 Gyro Registers //
////////////////////////////
#define ITG3701_XG_OFFS_TC_H     0x04
#define ITG3701_XG_OFFS_TC_L     0x05
#define ITG3701_YG_OFFS_TC_H     0x07
#define ITG3701_YG_OFFS_TC_L     0x08
#define ITG3701_ZG_OFFS_TC_H     0x0A
#define ITG3701_ZG_OFFS_TC_L     0x0B
#define ITG3701_XG_OFFS_USRH     0x13 // User-defined trim values for gyroscope
#define ITG3701_XG_OFFS_USRL     0x14
#define ITG3701_YG_OFFS_USRH     0x15
#define ITG3701_YG_OFFS_USRL     0x16
#define ITG3701_ZG_OFFS_USRH     0x17
#define ITG3701_ZG_OFFS_USRL     0x18
#define ITG3701_SMPLRT_DIV       0x19
#define ITG3701_CONFIG           0x1A
#define ITG3701_GYRO_CONFIG      0x1B
#define ITG3701_FIFO_EN          0x23
#define ITG3701_INT_PIN_CFG      0x37
#define ITG3701_INT_ENABLE       0x38
#define ITG3701_INT_STATUS       0x3A
#define ITG3701_TEMP_OUT_H       0x41
#define ITG3701_TEMP_OUT_L       0x42
#define ITG3701_GYRO_XOUT_H      0x43
#define ITG3701_GYRO_XOUT_L      0x44
#define ITG3701_GYRO_YOUT_H      0x45
#define ITG3701_GYRO_YOUT_L      0x46
#define ITG3701_GYRO_ZOUT_H      0x47
#define ITG3701_GYRO_ZOUT_L      0x48
#define ITG3701_USER_CTRL        0x6A
#define ITG3701_PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define ITG3701_PWR_MGMT_2       0x6C
#define ITG3701_FIFO_COUNTH      0x72
#define ITG3701_FIFO_COUNTL      0x73
#define ITG3701_FIFO_R_W         0x74
#define ITG3701_WHO_AM_I         0x75 // Should return 0x68

// ITG3701 Address
// Assumes AD0 is high
// If AD0 is brought low, uncomment next line
#define ITG3701_ADDRESS 0x68
//#define ITG3701_ADDRESS 0x69

class ITG3701
{
public:
  enum Gscale {
   GFS_500DPS,
   GFS_1000DPS,
   GFS_2000DPS,
   GFS_4000DPS
  };

  enum Godr {  // set of allowable gyro sample rates
   GODR_95Hz,
   GODR_190Hz,
   GODR_380Hz,
   GODR_760Hz
  };

  enum Gbw {   // set of allowable gyro data bandwidths
   GBW_low,      // 12.5 Hz at Godr = 95 Hz, 12.5 Hz at Godr = 190 Hz,  30 Hz at Godr = 760 Hz
   GBW_med,      // 25 Hz   at Godr = 95 Hz, 25 Hz   at Godr = 190 Hz,  35 Hz at Godr = 760 Hz
   GBW_high,     // 25 Hz   at Godr = 95 Hz, 50 Hz   at Godr = 190 Hz,  50 Hz at Godr = 760 Hz
   GBW_highest   // 25 Hz   at Godr = 95 Hz, 70 Hz   at Godr = 190 Hz, 100 Hz at Godr = 760 Hz
  };

  int16_t x, y, z; // x, y, and z axis readings of the gyroscope

  // Constructor
  ITG3701(uint8_t i2cAddress = ITG3701_ADDRESS);
  // Initialization
  void begin(
              Gscale m_gscale = GFS_500DPS,
              Godr m_godr = GODR_95Hz,
              Gbw m_gbw = GBW_low
            );

  void updateGyro();

private:
  // Setting variables
  uint8_t   m_i2cAddress;
  Gscale    m_gscale;
  Godr      m_godr;
  Gbw       m_gbw;
  // Wire.h read and write protocols
  void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data);
  uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);
  void I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
};
