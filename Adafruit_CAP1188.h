/*************************************************** 
  This is a library for the CAP1188 8-Channel Capacitive Sensor

  Designed specifically to work with the CAP1188 breakout from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

//Not using arduino so for now we get rid of this
/*
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
*/

//If using SPI library
#include <wiringPiSPI.h>

#include <stdint.h>

// The default I2C address
#define CAP1188_I2CADDR 0x29

// Some registers we use
#define CAP1188_SENINPUTSTATUS 0x3
#define CAP1188_MTBLK 0x2A
#define CAP1188_LEDLINK 0x72
#define CAP1188_PRODID 0xFD
#define CAP1188_MANUID 0xFE
#define CAP1188_STANDBYCFG 0x41
#define CAP1188_REV 0xFF
#define CAP1188_MAIN 0x00
#define CAP1188_MAIN_INT 0x01
#define CAP1188_LEDPOL 0x73


class Adafruit_CAP1188 {
 public:
  // Hardware I2C
  Adafruit_CAP1188(int8_t resetpin = -1);

  bool begin(int i2caddr = CAP1188_I2CADDR);
  int readRegister(int reg);
  void writeRegister(int reg, int value);
  int touched(void);
  void LEDpolarity(int x);

 private:
  bool _i2c;
  int8_t _i2caddr, _resetpin, _cs, _clk, _mosi, _miso;
};
