/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResourceIO.cpp
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_IO_CPP__
#define __ARDUINO_SIMPLE_RESOURCE_IO_CPP__ 1

#include "SimpleResourceIO.h"

SimpleResourceIO* SimpleResourceIO::association[RESOURCE_IO_DRIVERS_NUM];

SimpleResourceIO* SimpleResourceIO::getAssociatedIO(int driver) {
    return association[driver];
}

void SimpleResourceIO::associateIO(SimpleResourceIO* io, int driver) {
    association[driver] = io;
}

bool SimpleResourceIO::open() {
    return true;
}

int SimpleResourceIO::read(unsigned int address) {
    checkCache(address);
    if (validCacheSize < 1) {
        return -1;
    }
    return (int) cache[address - cacheMemoryAddress];
}

void SimpleResourceIO::write(unsigned int address, unsigned char b) {  
    checkCache(address);
    cache[address - cacheMemoryAddress] = b;
    wasCacheChanged = true;
}

void SimpleResourceIO::flush() {
    if (wasCacheChanged) {
        writeBytes(cacheMemoryAddress, cache, validCacheSize);
    }
}

void SimpleResourceIO::close() {
    flush();
}

#endif /* __ARDUINO_SIMPLE_RESOURCE_IO_CPP__ */

