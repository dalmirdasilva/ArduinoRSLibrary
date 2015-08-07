/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResourceSystem.cpp
 * 
 * This is the Resource system itself.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_SYSTEM_CPP__
#define __ARDUINO_SIMPLE_RESOURCE_SYSTEM_CPP__ 1

#include "SimpleResourceSystem.h"

SimpleResourceSystem::SimpleResourceSystem(int driver) {
    lastOperationResult = Resource::OPERATION_SUCCESS;
    rs.driver = (rs_driver_t) driver;
}

bool SimpleResourceSystem::mount(MountOptions options) {
    lastOperationResult = (Resource::ResourceOperationResult) rs_mount(rs.driver, &rs, (rs_mount_options_t) options);
    return (lastOperationResult == Resource::OPERATION_SUCCESS);
}

bool SimpleResourceSystem::umount() {
    SimpleResourceIO::getAssociatedIO(rs.driver)->flush();
    lastOperationResult = (Resource::ResourceOperationResult) rs_umount(&rs);
    return (lastOperationResult == Resource::OPERATION_SUCCESS);
}

SimpleResource SimpleResourceSystem::alloc() {
    SimpleResource rw(RS_NULL_RESOURCE_CODE, &rs);
    rs_resource_code_t code;
    code = rs_alloc(&rs);
    if (code != RS_NULL_RESOURCE_CODE) {
        rw.setCode(code);
    }
    return rw;
}

SimpleResource SimpleResourceSystem::getResourceByCode(int code) {
    SimpleResource rw((rs_resource_code_t) code, &rs);
    return rw;
}

unsigned int SimpleResourceSystem::totalSpace() {
    return (unsigned int) rs_total_space(&rs);
}

unsigned int SimpleResourceSystem::availableSpace() {
    return (unsigned int) rs_available_space(&rs);
}

#endif /* __ARDUINO_SIMPLE_RESOURCE_SYSTEM_CPP__ */
