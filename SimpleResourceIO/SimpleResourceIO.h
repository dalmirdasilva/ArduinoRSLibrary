/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResourceIO.h
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_IO_H__
#define __ARDUINO_SIMPLE_RESOURCE_IO_H__ 1

#include <ResourceIO.h>

#define RESOURCE_IO_CACHE_SIZE 8
#define RESOURCE_IO_DRIVERS_NUM 5

class SimpleResourceIO : public ResourceIO {
private:

    static SimpleResourceIO* association[RESOURCE_IO_DRIVERS_NUM];
    bool wasCacheChanged, wasCacheInitialized;
    unsigned int cacheMemoryAddress;
    unsigned char cache[RESOURCE_IO_CACHE_SIZE];
    unsigned int cacheMiss, cacheHit;
    unsigned int validCacheSize;
    
    void checkCache(unsigned int address) {
        if (!wasCacheInitialized || (address < cacheMemoryAddress || address >= (cacheMemoryAddress + validCacheSize))) {
            flush();
            validCacheSize = readBytes(address, cache, RESOURCE_IO_CACHE_SIZE);
            cacheMemoryAddress = address;
            wasCacheChanged = false;
            wasCacheInitialized = true;
            cacheMiss++;
        } else {
            cacheHit++;
        }
    }

protected:

    SimpleResourceIO() {
        cacheMiss = 0;
        cacheHit = 0;
        cacheMemoryAddress = 0;
        wasCacheChanged = false;
        wasCacheInitialized = false;
        validCacheSize = 0;
    }

    virtual int readBytes(unsigned int address, unsigned char* buf, int len) {
    }

    virtual void writeBytes(unsigned int address, unsigned char* buf, int len) {
    }

public:

    static void associateIO(SimpleResourceIO* io, int driver);

    static SimpleResourceIO* getAssociatedIO(int driver);

    virtual bool open();

    virtual int read(unsigned int address);

    virtual void write(unsigned int address, unsigned char b);

    virtual void flush();

    virtual void close();

    unsigned int getCacheHit() {
        return cacheHit;
    }

    unsigned int getCacheMiss() {
        return cacheMiss;
    }
};

#endif /* __ARDUINO_SIMPLE_RESOURCE_IO_H__ */

