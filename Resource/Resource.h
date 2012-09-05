/**
 * Arduino - Resource interface
 * 
 * Resource.h
 * 
 * This is a resource interface
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_RESOURCE_H__
#define __ARDUINO_RESOURCE_H__ 1

class Resource {
public:
    
    enum ResourceOperationResult {
        OPERATION_SUCCESS = 0,
        OPERATION_ERROR_RESOURCE_OPENED = 1,
        OPERATION_ERROR_RESOURCE_CLOSED = 2,
        OPERATION_ERROR_RESOURCE_READ_ONLY = 3,
        OPERATION_ERROR_NO_SPACE_AVAILABLE = 4,
        OPERATION_ERROR_DRIVER_BUSY = 5,
        OPERATION_ERROR_SEEK_OUT_OF_BOUND = 6,
        OPERATION_ERROR_RESOURCE_DOES_NOT_ALLOCATED = 7,
        OPERATION_ERROR_DRIVER_NOT_MOUNTED = 8
    };
    
    enum OpenOptions {
        OPEN_READ_WRITE = 0,
        OPEN_READ_ONLY = 1
    };
    
    enum ResourceSeekOrigin {
        SEEK_ORIGIN_BEGIN = 0,
        SEEK_ORIGIN_CURRENT = 1
    };

    virtual bool open(OpenOptions options) = 0;

    virtual bool close() = 0;

    virtual void write(unsigned char b) = 0;

    virtual void writeBytes(unsigned char* buf, int len) = 0;

    virtual int read() = 0;

    virtual int readBytes(unsigned char* buf, int len) = 0;

    virtual bool seek(ResourceSeekOrigin origin, unsigned int offset) = 0;

    virtual bool truncate() = 0;

    virtual void sync() = 0;

    virtual bool rewind() = 0;

    virtual void release() = 0;

    virtual unsigned int size() = 0;

    virtual unsigned int tell() = 0;

    virtual bool eor() = 0;

    virtual bool error() = 0;

    virtual bool isReadOnly() = 0;
};

#endif // __ARDUINO_RESOURCE_H__
