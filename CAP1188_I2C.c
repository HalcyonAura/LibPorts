/*
 * Author: Cecilia La Place
 * Made using Adafruit_CAP1188 Arduino Library as a reference
 */

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <stdio.h>

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET 29

// The default I2C address
#define CAP1188_I2CADDR 0x28

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

bool _i2c;
int _i2caddr, _fd;


bool begin(uint8_t i2caddr) {
  _fd = wiringPiI2CSetup(i2caddr);
  _i2caddr = i2caddr;
  if (CAP1188_RESET != -1) {
    pinMode(CAP1188_RESET, OUTPUT);
    digitalWrite(CAP1188_RESET, LOW);
    delay(100);
    digitalWrite(CAP1188_RESET, HIGH);
    delay(100);
    digitalWrite(CAP1188_RESET, LOW);
    delay(100);
  }

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

uint8_t touched(void) {
  uint8_t t = readRegister(CAP1188_SENINPUTSTATUS);
  if (t) {
    writeRegister(CAP1188_MAIN, readRegister(CAP1188_MAIN) & ~CAP1188_MAIN_INT);
  }
  return t;
}

void LEDpolarity(uint8_t x) {
  writeRegister(CAP1188_LEDPOL, x);
}

uint8_t readRegister(uint8_t reg) {
    return wiringPiI2CReadReg8(_fd, reg);
}

void writeRegister(uint8_t reg, uint8_t value) {
    wiringPiI2CWriteReg8(_fd, reg, value);
}

void setup(int resetpin) {
  wiringPiSetup();
  // Initialize the sensor using I2C address
  if (!begin(0x28, resetpin)) {
  //if (!cap.begin()) {
    printf("CAP1188 not found\n");
  }
  else{
    printf("CAP1188 found!\n");
  }

}

int main() {
  while(1){
    setup();
    uint8_t touch = touched();
    if (touch!=0){
      for (uint8_t i=0; i<8; i++) {
        if (touch & (1 << i)) {
          printf("C"); 
          printf("%d", (i+1)); 
          printf("\t");
        }
      }
      printf("\n");
      delay(50);
    }
  }
  return 0;
}