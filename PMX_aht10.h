#ifndef __PMX_AHT10_H
#define __PMX_AHT10_H

#include <Wire.h>
#include <Arduino.h>


#define AHT10_ADDRESS_0X38         0x38  //chip I2C address no.1 for AHT10/AHT15/AHT20, address pin connected to GND
#define AHT10_ADDRESS_0X39         0x39  //chip I2C address no.2 for AHT10 only, address pin connected to Vcc

#define AHT10_INIT_CMD             0xE1  //initialization command for AHT10/AHT15
#define AHT20_INIT_CMD             0xBE  //initialization command for AHT20
#define AHT10_START_MEASURMENT_CMD 0xAC  //start measurment command
#define AHT10_NORMAL_CMD           0xA8  //normal cycle mode command, no info in datasheet!!!
#define AHT10_SOFT_RESET_CMD       0xBA  //soft reset command

#define AHT10_INIT_NORMAL_MODE     0x00  //enable normal mode
#define AHT10_INIT_CYCLE_MODE      0x20  //enable cycle mode
#define AHT10_INIT_CMD_MODE        0x40  //enable command mode
#define AHT10_INIT_CAL_ENABLE      0x08  //load factory calibration coeff


#define AHT10_DATA_MEASURMENT_CMD  0x33  //no info in datasheet!!! my guess it is DAC resolution, saw someone send 0x00 instead
#define AHT10_DATA_NOP             0x00  //no info in datasheet!!!


#define AHT10_MEASURMENT_DELAY     80    //at least 75 milliseconds
#define AHT10_POWER_ON_DELAY       40    //at least 20..40 milliseconds
#define AHT10_CMD_DELAY            350   //at least 300 milliseconds, no info in datasheet!!!
#define AHT10_SOFT_RESET_DELAY     20    //less than 20 milliseconds

#define AHT10_FORCE_READ_DATA      true  //force to read data
#define AHT10_USE_READ_DATA        false //force to use data from previous read
#define AHT10_ERROR                0xFF  //returns 255, if communication error is occurred


typedef enum : uint8_t
{
  AHT10_SENSOR = 0x00,
  AHT15_SENSOR = 0x01,
  AHT20_SENSOR = 0x02
}
ASAIR_I2C_SENSOR;


class AHT10
{
  public:

   AHT10(uint8_t address = AHT10_ADDRESS_0X38, ASAIR_I2C_SENSOR = AHT10_SENSOR);
   bool     begin();
   uint8_t  readRawData();
   float    readTemperature(bool readI2C = AHT10_FORCE_READ_DATA);
   float    readHumidity(bool readI2C = AHT10_FORCE_READ_DATA);
   bool     softReset();
   bool     setNormalMode();
   bool     setCycleMode();

  private:
   uint8_t          _address;
   ASAIR_I2C_SENSOR _sensorName;
   uint8_t          _rawDataBuffer[6] = {AHT10_ERROR, 0, 0, 0, 0, 0};

   uint8_t  readStatusByte();
   uint8_t  getCalibrationBit(bool readI2C = AHT10_FORCE_READ_DATA);
   bool     enableFactoryCalCoeff();
   uint8_t  getBusyBit(bool readI2C = AHT10_FORCE_READ_DATA);
};


#endif