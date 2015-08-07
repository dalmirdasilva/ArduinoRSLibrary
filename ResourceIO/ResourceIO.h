/**
 * Arduino - Resource interface
 * 
 * ResourceIO.h
 * 
 * This is a resource IO interface
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_RESOURCE_IO_H__
#define __ARDUINO_RESOURCE_IO_H__ 1

class ResourceIO {
public:

    virtual bool open() = 0;

    virtual int read(unsigned int address) = 0;

    virtual void write(unsigned int address, unsigned char b) = 0;

    virtual void flush() = 0;

    virtual void close() = 0;
};

#endif /* __ARDUINO_RESOURCE_IO_H__ */

