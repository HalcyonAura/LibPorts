/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_CAP1188.h"
#include <wiringPi.h>
#include <stdio.h>

Adafruit_CAP1188::Adafruit_CAP1188(int8_t resetpin) {
  // I2C
  _resetpin = resetpin;
  _i2c = true;
}

Adafruit_CAP1188::Adafruit_CAP1188(int8_t cspin, int8_t resetpin) {
  // Hardware SPI
  _cs = cspin;
  _resetpin = resetpin;
  _clk = -1;
  _i2c = false;
}

bool Adafruit_CAP1188::begin(int i2caddr) {
  //I2C implementation
  if (_i2c) {
    //Wire.begin();
    wiringPiI2CSetup(i2caddr);
    _i2caddr = i2caddr;
  } 

  if (_resetpin != -1) {
    pinMode(_resetpin, OUTPUT);
    digitalWrite(_resetpin, LOW);
    delay(100);
    digitalWrite(_resetpin, HIGH);
    delay(100);
    digitalWrite(_resetpin, LOW);
    delay(100);
  }

  readRegister(CAP1188_PRODID);
  
  if ( (readRegister(CAP1188_PRODID) != 0x50) ||
       (readRegister(CAP1188_MANUID) != 0x5D) ||
       (readRegister(CAP1188_REV) != 0x83)) {
    return false;
  }
  // allow multiple touches
  writeRegister(CAP1188_MTBLK, 0); 
  // Have LEDs follow touches
  writeRegister(CAP1188_LEDLINK, 0xFF);
  // speed up a bit
  writeRegister(CAP1188_STANDBYCFG, 0x30);
  return true;
}

int  Adafruit_CAP1188::touched(void) {
  int t = readRegister(CAP1188_SENINPUTSTATUS);
  if (t) {
    writeRegister(CAP1188_MAIN, readRegister(CAP1188_MAIN) & ~CAP1188_MAIN_INT);
  }
  return t;
}

void Adafruit_CAP1188::LEDpolarity(int x) {
  writeRegister(CAP1188_LEDPOL, x);
}

int Adafruit_CAP1188::readRegister(int reg) {
  if (_i2c) {
    /*
    Wire.beginTransmission(_i2caddr);
    i2cwrite(reg);
    Wire.endTransmission();
    Wire.requestFrom(_i2caddr, 1);
    return (i2cread());
    */
    return wiringPiI2CReadReg8(_i2c, reg);
  } 
}


/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void Adafruit_CAP1188::writeRegister(int reg, int value) {
  if (_i2c) {
    /*Wire.beginTransmission(_i2caddr);
    i2cwrite((int)reg);
    i2cwrite((int)(value));
    Wire.endTransmission();*/
    wiringPiI2CWriteReg8(_i2c, reg, value);
  } 
}