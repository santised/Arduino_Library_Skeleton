#ifndef _CLASS_NAME_H
#define _CLASS_NAME_H

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>

typedef uint16_t i2cAddress;
const i2cAddress defAddr = 0x00;

enum {
};

class Class_Name
{  
  public:

    className constructor(); // SPI Constructor
    className constructor(uint16_t address); // I2C Constructor

    bool begin( TwoWire &wirePort );
    bool beginSpi(uint8_t userCsPin, SPIClass &spiPort);

    bool begin(TwoWire &wirePort = Wire); // begin function
    bool beginSpi(SPIClass &spiPort = SPI); 

  private:

    uint16_t _address;

    // This generic function reads an eight bit register. It takes the register's
    // address as its' parameter. 
    uint8_t readRegister(uint8_t _reg);

    // This generic function handles I2C write commands for modifying individual
    // bits in an eight bit register. Paramaters include the register's address, a mask 
    // for bits that are ignored, the bits to write, and the bits' starting
    // position.
    void writeRegister(uint8_t _wReg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition);

    TwoWire *_i2cPort;
    SPIClass *_spiPort;
};
#endif
