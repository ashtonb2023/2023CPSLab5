#ifndef LM75B_H
#define LM75B_H

#include "mbed.h"

//  LM75B IIC address
#define    LM75B_ADDR 0x90

//  LM75B registers
#define    LM75B_Conf        0x01
#define    LM75B_Temp        0x00
#define    LM75B_Tos         0x03
#define    LM75B_Thyst       0x02

//!Library for the LM75B temperature sensor.
/*!
The LM75B is an I2C digital temperature sensor, with a range of -55C to +125C and a 0.125C resolution.
*/
class LM75B
{
public:
  //!Creates an instance of the class.
  /*!
  Connect module at I2C address addr using I2C port pins sda and scl.
  LM75B
  */
  LM75B(PinName sda, PinName scl);
  
  /*!
  Destroys instance.
  */ 
  ~LM75B();
  
  //!Reads the current temperature.
  /*!
  Reads the temperature register of the LM75B and converts it to a useable value.
  */
  float read();
  
private:
  
  I2C i2c;

};

#endif