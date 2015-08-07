/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResource.h
 * 
 * This is the Resource representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_H__
#define __ARDUINO_SIMPLE_RESOURCE_H__ 1

#include <Resource.h>
#include <SimpleResource.h>
#include <SimpleResourceIO.h>
#include <rs.h>

class SimpleResource : public Resource {
    rs_resource_code_t code;
    rs_resource_t resource;
    rs_t* rs;
    ResourceOperationResult lastOperationResult;
public:

    SimpleResource(rs_resource_code_t code, rs_t* rs);
    
    ResourceOperationResult getLastOperationResult() {
        return lastOperationResult;
    } 

    virtual void setCode(int code) {
        this->code = (rs_resource_code_t) code;
    }

    virtual int getCode() {
        return (int) this->code;
    }

    virtual bool open(OpenOptions options);

    virtual bool close();

    virtual void write(unsigned char b);

    virtual void writeBytes(unsigned char* buf, int len);

    virtual int read();

    virtual int readBytes(unsigned char* buf, int len);

    virtual bool seek(ResourceSeekOrigin origin, unsigned int offset);

    virtual bool truncate();

    virtual void sync();

    virtual bool rewind();

    virtual void release();

    virtual unsigned int size();

    virtual unsigned int tell();

    virtual bool eor();

    virtual bool error();

    virtual bool isReadOnly();
};

#endif // __ARDUINO_SIMPLE_RESOURCE_H__
