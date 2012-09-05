/**
 * SDCC - PIC resource system
 * 
 * rs.h
 * 
 * An file system header definition based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_H__
#define __SDCC_RS_H__ 1

#include <stdint.h>

#define RS_NULL_RESOURCE_CODE                                           0xff
#define RS_NULL_CLUSTER                                                 0xff

#define RS_NULL_RESORCE_DESCRIPTOR_ADDRESS                              0xff
#define RS_NULL_CLUSTER_ADDRESS                                         0x00

#define RS_FIRST_ADDRESS_OF_MEMORY                                      0x00

#define RS_SIZEOF_RESOURCE_SIZE                                         0x02

#define RS_INEXISTENT_CLUSTER                                           0xff

#define CLUSTER_ADDRESS_TO_NEXT(CLUSTER_ADDRESS)                        ((CLUSTER_ADDRESS) + 0)
#define CLUSTER_ADDRESS_TO_PREV(CLUSTER_ADDRESS)                        ((CLUSTER_ADDRESS) + 1)
#define CLUSTER_ADDRESS_TO_DATA(CLUSTER_ADDRESS)                        ((CLUSTER_ADDRESS) + 2)

#define RD_ADDRESS_TO_SIZE_LOW(RD_ADDRESS)                              ((RD_ADDRESS) + 0)
#define RD_ADDRESS_TO_SIZE_HIGH(RD_ADDRESS)                             ((RD_ADDRESS) + 1)
#define RD_ADDRESS_TO_FIRST_CLUSTER(RD_ADDRESS)                         ((RD_ADDRESS) + 2)
#define RD_ADDRESS_TO_FLAG(RD_ADDRESS)                                  ((RD_ADDRESS) + 3)

typedef uint8_t rs_resource_descriptor_t;
typedef uint8_t rs_cluster_t;
typedef uint16_t rs_resource_size_t;
typedef uint16_t rs_memory_address_t;
typedef uint8_t rs_resource_code_t;
typedef uint16_t rs_seek_int_t;

// Drivers

typedef enum {
    RS_DRIVER_VIRTUAL = 0,
    RS_DRIVER_SELF_EEPROM = 1,
    RS_DRIVER_MULTI_EXTERNAL_EEPROM = 2,
    RS_DRIVER_EXTERNAL_EEPROM = 3,
    RS_DRIVER_ARDUINO_EEPROM = 4
} rs_driver_t;

// Resource fag bit values

typedef enum {
    RS_RESOURCE_FLAG_BIT_OPENED = 1,
    RS_RESOURCE_FLAG_BIT_READ_ONLY = 2,
    RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ = 4,
    RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE = 8,
    RS_RESOURCE_FLAG_BIT_ALLOCATED = 16,
    RS_RESOURCE_FLAG_BIT_EOR_REACHED = 32
} rs_resource_flag_bits_t;

// Options to open a resource

typedef enum {
    RS_OPEN_RESOURCE_OPTION_NORMAL = 0,
    RS_OPEN_RESOURCE_OPTION_READ_ONLY = 1
} rs_open_resource_options_t;

// Options to mount a resource

typedef enum {
    RS_MOUNT_OPTION_NORMAL = 0,
    RS_MOUNT_OPTION_READ_ONLY = 1
} rs_mount_options_t;

// Rs fag bit values

typedef enum {
    RS_FLAG_BIT_DRIVER_MOUNTED = 1,
    RS_FLAG_BIT_READ_ONLY = 2
} rs_flag_bits_t;

// Operation result

typedef enum {
    RS_OP_RESULT_SUCCESS = 0,
    RS_OP_RESULT_ERROR_RESOURCE_OPENED = 1,
    RS_OP_RESULT_ERROR_RESOURCE_CLOSED = 2,
    RS_OP_RESULT_ERROR_RESOURCE_READ_ONLY = 3,
    RS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE = 4,
    RS_OP_RESULT_ERROR_DRIVER_BUSY = 5,
    RS_OP_RESULT_ERROR_SEEK_OUT_OF_BOUND = 6,
    RS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED = 7,
    RS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED = 8
} rs_op_result_t;

// Seek position reference

typedef enum {
    RS_SEEK_ORIGIN_BEGIN = 0,
    RS_SEEK_ORIGIN_CURRENT = 1
} rs_seek_origin_t;

typedef struct {
    uint8_t flags;
} rs_stat_t;

// Resource system

typedef struct {
    rs_driver_t driver;
    uint16_t memory_size;
    rs_memory_address_t resource_descriptor_table_address;
    rs_memory_address_t cluster_table_address;
    uint16_t sizeof_resource_descriptor_table;
    uint16_t sizeof_cluster_table;
    uint8_t sizeof_resource_descriptor;
    uint8_t sizeof_cluster;
    uint8_t resource_descriptor_count;
    uint8_t cluster_count;
    uint8_t sizeof_cluster_data;
    uint8_t sizeof_cluster_control;
    uint8_t free_clusters;
    uint8_t flags;
} rs_t;

// Resource

typedef struct {
    rs_resource_descriptor_t resource_descriptor;
    rs_cluster_t first_cluster;
    rs_cluster_t current_cluster;
    uint8_t cluster_offset;
    uint16_t size;
    uint16_t current_position;
    uint8_t flags;
} rs_resource_t;

typedef struct {
    uint8_t driver_mouted;
} rs_global_flags_t;

extern rs_global_flags_t rs_global_flags;

// Format a device
rs_op_result_t rs_format(rs_t *rs);

// Register a work area
rs_op_result_t rs_mount(rs_driver_t driver, rs_t *rs, rs_mount_options_t options);

// Unregister a work area
rs_op_result_t rs_umount(rs_t *rs);

// Open/Create a resource (you must give a empty resource)
rs_op_result_t rs_open(rs_t *rs, rs_resource_code_t resource_code, rs_resource_t *resource, rs_open_resource_options_t options);

// Close a resource
rs_op_result_t rs_close(rs_t *rs, rs_resource_t *resource);

// Read a byte from resource
uint8_t rs_read(rs_t *rs, rs_resource_t *resource);

// Write a byte from resource
rs_op_result_t rs_write(rs_t *rs, rs_resource_t *resource, uint8_t data_to_write);

// Move read/write pointer, (Expand resource size not implemented yet)
rs_op_result_t rs_seek(rs_t *rs, rs_resource_t *resource, rs_seek_origin_t origin, rs_seek_int_t offset);

// Truncate resource size
rs_op_result_t rs_truncate(rs_t *rs, rs_resource_t *resource);

// Flush cached data
void rs_sync(rs_t *rs, rs_resource_t *resource);

// Get descriptor status
void rs_stat(rs_t *rs, rs_resource_t *resource, rs_stat_t *stat);

// Rewind the position of a resource pointer
rs_op_result_t rs_rewind(rs_t *rs, rs_resource_t *resource);

// Create/Allocate a new resource if available
rs_resource_code_t rs_alloc(rs_t *rs);

// Make a resource free to be allocated for another one
uint8_t rs_release(rs_t *rs, rs_resource_t *resource);

// Get size of a resource
rs_resource_size_t rs_size(rs_resource_t *resource);

// Get the current read/write pointer
rs_resource_size_t rs_tell(rs_resource_t *resource);

// Test for end-of-resource on a resource
uint8_t rs_eor(rs_resource_t *resource);

// Test for an error on a resource
uint8_t rs_error(rs_resource_t *resource);

// Return the current available space in the partition
rs_resource_size_t rs_available_space(rs_t *rs);

// Return the total space in the partition
rs_resource_size_t rs_total_space(rs_t *rs);

#endif // __SDCC_RS_H__
