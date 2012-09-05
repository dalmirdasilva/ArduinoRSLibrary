/**
 * Arduino - A simple resource implementation
 * 
 * SimpleArrayResourceIO.h
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_H__
#define __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_H__ 1

#include <SimpleResourceIO.h>

class SimpleArrayResourceIO : public SimpleResourceIO {
private:
    unsigned char* array;
    unsigned int size;
public:

    SimpleArrayResourceIO(unsigned char* array, unsigned int size);

protected:
    
    virtual int readBytes(unsigned int address, unsigned char* buf, int len);

    virtual void writeBytes(unsigned int address, unsigned char* buf, int len);
};

#endif	/* __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_H__ */

