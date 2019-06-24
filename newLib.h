/*
  This is a library for...
  By: Elias Santistevan
  Date: 
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "Class_Name.h"

Class_Name::Class_Name(){} //Constructor for SPI
Class_Name::Class_Name(uint16_t address){  _address = address; } //Constructor for I2C

bool Class_Name::begin( TwoWire &wirePort )
{
  
  _i2cPort = &wirePort;

  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if( !_ret )
    return true; 
  else 
    return false; 

}

bool Class_Name::beginSpi(uint8_t userCsPin, SPIClass &spiPort)
{
  _i2cPort = NULL;

  _spiPort = &spiPort;

  _cs = userCsPin;
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  _spiPort.begin();
#ifdef ESP32 
    SPI.setBitOrder(SPI_MSBFIRST);
#else
    SPI.setBitOrder(MSBFIRST);
#endif

  return true; 
}

// This generic function handles I2C write commands for modifying individual
// bits in an eight bit register. Paramaters include the register's address, a mask 
// for bits that are ignored, the bits to write, and the bits' starting
// position.
void Class_Name::writeRegister(uint8_t _wReg, uint8_t _mask, uint8_t _bits, uint8_t _startPosition)
{
  if(_i2cPort == NULL) {
    _spiWrite = readRegister(_wReg); // Get the current value of the register
    _spiWrite &= (~_mask); // Mask the position we want to write to
    _spiWrite |= (_bits << _startPosition); // Write the given bits to the variable
    _spiPort->beginTransaction(SPISettings(_spiPortSpeed, MSBFIRST, SPI_MODE1)); 
    digitalWrite(_cs, LOW); // Start communication
    _spiPort->transfer(_wReg); // Start write command at given register
    _spiPort->transfer(_spiWrite); // Write to register
    digitalWrite(_cs, HIGH); // End communcation
    _spiPort->endTransaction();
  }
  else { 
    _i2cWrite = readRegister(_wReg); // Get the current value of the register
    _i2cWrite &= (~_mask); // Mask the position we want to write to.
    _i2cWrite |= (_bits << _startPosition);  // Write the given bits to the variable
    _i2cPort->beginTransmission(_address); // Start communication.
    _i2cPort->write(_wReg); // at register....
    _i2cPort->write(_i2cWrite); // Write register...
    _i2cPort->endTransmission(); // End communcation.
  }
}

// This generic function reads an eight bit register. It takes the register's
// address as its' parameter. 
uint8_t Class_Name::readRegister(uint8_t _reg)
{

  if(_i2cPort == NULL) {
    _spiPort->beginTransaction(SPISettings(_spiPortSpeed, MSBFIRST, SPI_MODE1)); 
    digitalWrite(_cs, LOW); // Start communication.
    _spiPort->transfer(_reg |= SPI_READ_M);  // Register OR'ed with SPI read command. 
    _regValue = _spiPort->transfer(0); // Get data from register.  
    _spiPort->endTransaction();
    return(_regValue); 
  }
  else {
    _i2cPort->beginTransmission(_address); 
    _i2cPort->write(_reg); // Moves pointer to register.
    _i2cPort->endTransmission(false); // 'False' here sends a restart message so that bus is not released
    _i2cPort->requestFrom(_address, 1); // Read the register, only ever once. 
    _regValue = _i2cPort->read();
    return(_regValue);
  }
}

