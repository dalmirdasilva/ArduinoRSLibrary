/**
 * SDCC - PIC resource system
 * 
 * rs_init_partition.h
 * 
 * Initializes a rs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_INIT_PARTITION_H__
#define __SDCC_RS_INIT_PARTITION_H__ 1

#include "rs.h"

typedef enum {
    RS_DISK_4K,
    RS_DISK_8K,
    RS_DISK_24K,
    RS_DISK_32K
} rs_disk_size_t;

typedef enum {
    RS_ENV_ARDUINO,
    RS_ENV_VIRTUAL
} rs_environment_t;

void rs_init_partition(rs_t *rs, rs_disk_size_t size, rs_environment_t env);

#endif /* __SDCC_RS_INIT_PARTITION_H__ */
