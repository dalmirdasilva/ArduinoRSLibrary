/**
 * Arduino - A simple resource implementation
 * 
 * SimpleExternalEepromResourceIO.cpp
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_CPP__
#define __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_CPP__ 1

#include "SimpleExternalEepromResourceIO.h"

SimpleExternalEepromResourceIO::SimpleExternalEepromResourceIO(ExternalEeprom* externalEeprom) : SimpleResourceIO(), externalEeprom(externalEeprom) {
}

int SimpleExternalEepromResourceIO::readBytes(unsigned int address, unsigned char* buf, int len) {
    return externalEeprom->readBytes(address, buf, len);
}

void SimpleExternalEepromResourceIO::writeBytes(unsigned int address, unsigned char* buf, int len) {
    externalEeprom->writeBytes(address, buf, len);
}

#endif	/* __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_CPP__ */

