/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResource.cpp
 * 
 * This is the Resource representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_CPP__
#define __ARDUINO_SIMPLE_RESOURCE_CPP__ 1

#include "SimpleResource.h"

SimpleResource::SimpleResource(rs_resource_code_t code, rs_t* rs) : code(code), rs(rs) {
    lastOperationResult = OPERATION_SUCCESS;
}

bool SimpleResource::open(OpenOptions options) {
    lastOperationResult = (ResourceOperationResult) rs_open(rs, code, &resource, (rs_open_resource_options_t) options);
    return (lastOperationResult == OPERATION_SUCCESS);
}

bool SimpleResource::close() {
    sync();
    lastOperationResult = (ResourceOperationResult) rs_close(rs, &resource);
    return (lastOperationResult == OPERATION_SUCCESS);
}

void SimpleResource::write(unsigned char b) {
    lastOperationResult = (ResourceOperationResult) rs_write(rs, &resource, b);
}

void SimpleResource::writeBytes(unsigned char* buf, int count) {
    lastOperationResult = OPERATION_SUCCESS;
    for (int i = 0; i < count && lastOperationResult == OPERATION_SUCCESS; i++) {
        write(buf[i]);
    }
}

int SimpleResource::read() {
    if (eor()) {
        return -1;
    }
    return rs_read(rs, &resource);
}

int SimpleResource::readBytes(unsigned char* buf, int count) {
    int i, c;
    if (buf == (unsigned char*) 0) {
        return 0;
    }
    c = read();
    if (c == -1) {
        return -1;
    }
    buf[0] = c;
    for (i = 1; i < count; i++) {
        c = read();
        if (c == -1) {
            break;
        }
        buf[i] = c;
    }
    return i;
}

bool SimpleResource::seek(ResourceSeekOrigin origin, unsigned int offset) {
    lastOperationResult = (ResourceOperationResult) rs_seek(rs, &resource, (rs_seek_origin_t) origin, (rs_seek_int_t) offset);
    return (lastOperationResult == OPERATION_SUCCESS);
}

bool SimpleResource::truncate() {
    lastOperationResult = (ResourceOperationResult) rs_truncate(rs, &resource);
    return (lastOperationResult == OPERATION_SUCCESS);
}

void SimpleResource::sync() {
    rs_sync(rs, &resource);
    SimpleResourceIO::getAssociatedIO(rs->driver)->flush();
}

bool SimpleResource::rewind() {
    lastOperationResult = (ResourceOperationResult) rs_rewind(rs, &resource);
    return (lastOperationResult == OPERATION_SUCCESS);
}

void SimpleResource::release() {
    sync();
    rs_release(rs, &resource);
}

unsigned int SimpleResource::size() {
    return (unsigned int) rs_size(&resource);
}

unsigned int SimpleResource::tell() {
    return (unsigned int) rs_tell(&resource);
}

bool SimpleResource::eor() {
    return (rs_eor(&resource) != 0);
}

bool SimpleResource::error() {
    return (rs_error(&resource) != 0);
}

bool SimpleResource::isReadOnly() {
    return (rs->flags & RS_RESOURCE_FLAG_BIT_READ_ONLY) != 0;
}

#endif /* __ARDUINO_SIMPLE_RESOURCE_CPP__ */
