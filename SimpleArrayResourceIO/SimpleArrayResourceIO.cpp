/**
 * Arduino - A simple resource implementation
 * 
 * SimpleArrayResourceIO.cpp
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_CPP__
#define __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_CPP__ 1

#include "SimpleArrayResourceIO.h"

SimpleArrayResourceIO::SimpleArrayResourceIO(unsigned char* array, unsigned int size) : SimpleResourceIO(), array(array), size(size) {
}

int SimpleArrayResourceIO::readBytes(unsigned int address, unsigned char* buf, int len) {
    unsigned int available = (size - address);
    if (available < 1) {
        return -1;
    }
    len = (len > available) ? available : len;
    for (int i = 0; i < len; i++) {
        buf[i] = array[address + i];
    }
    return len;
}

void SimpleArrayResourceIO::writeBytes(unsigned int address, unsigned char* buf, int len) {
    for (int i = 0; i < len && (address + i) < size; i++) {
        array[address + i] = buf[i];
    }
}

#endif	/* __ARDUINO_SIMPLE_ARRAY_RESOURCE_IO_CPP__ */

