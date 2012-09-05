/**
 * Arduino - ResourceSystem interface
 * 
 * ResourceSystem.h
 * 
 * This is a resource system interface
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_RESOURCE_SYSTEM_H__
#define __ARDUINO_RESOURCE_SYSTEM_H__ 1

#include <Resource.h>

class ResourceSystem {
public:
    
    enum MountOptions {
        MOUNT_READ_WRITE = 0,
        MOUNT_READ_ONLY = 1
    };
    
    virtual bool mount(MountOptions options) = 0;

    virtual bool umount() = 0;
    
    virtual unsigned int totalSpace() = 0;
    
    virtual unsigned int availableSpace() = 0;
};

#endif /* __ARDUINO_RESOURCE_SYSTEM_H__ */
