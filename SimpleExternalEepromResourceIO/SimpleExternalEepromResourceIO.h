/**
 * Arduino - A simple resource implementation
 * 
 * SimpleExternlaEepromResourceIO.h
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_H__
#define __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_H__ 1

#include <ExternalEeprom.h>
#include <SimpleResourceIO.h>

class SimpleExternalEepromResourceIO : public SimpleResourceIO {
private:
    ExternalEeprom* externalEeprom;
public:

    SimpleExternalEepromResourceIO(ExternalEeprom* externalEeprom);

protected:

    virtual int readBytes(unsigned int address, unsigned char* buf, int len);

    virtual void writeBytes(unsigned int address, unsigned char* buf, int len);
};

#endif	/* __ARDUINO_SIMPLE_EXTERNAL_EEPROM_RESOURCE_IO_H__ */

