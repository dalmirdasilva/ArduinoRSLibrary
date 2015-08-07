/**
 * Arduino - A simple resource implementation
 * 
 * SimpleResourceSystem.h
 * 
 * This is the Resource system itself.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_RESOURCE_SYSTEM_H__
#define __ARDUINO_SIMPLE_RESOURCE_SYSTEM_H__ 1

#include <Resource.h>
#include <ResourceSystem.h>
#include <SimpleResource.h>
#include <rs.h>

class SimpleResourceSystem : public ResourceSystem {
    rs_t rs;
    Resource::ResourceOperationResult lastOperationResult;
public:

    SimpleResourceSystem(int driver);

    static bool format(rs_t* rs) {
        Resource::ResourceOperationResult o = (Resource::ResourceOperationResult) rs_format(rs);
        return (o == Resource::OPERATION_SUCCESS);
    }

    rs_t* getRs() {
        return &rs;
    }

    Resource::ResourceOperationResult getLastOperationResult() {
        return lastOperationResult;
    }

    virtual bool mount(MountOptions options);

    virtual bool umount();

    SimpleResource alloc();

    SimpleResource getResourceByCode(int code);

    virtual unsigned int totalSpace();

    virtual unsigned int availableSpace();
};

#endif // __ARDUINO_SIMPLE_RESOURCE_SYSTEM_H__
