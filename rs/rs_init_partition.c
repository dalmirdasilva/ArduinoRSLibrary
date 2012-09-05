/**
 * SDCC - PIC resource system
 * 
 * rs_init_partition.c
 * 
 * Initializes a rs partition
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_INIT_PARTITION_C__
#define __SDCC_RS_INIT_PARTITION_C__ 1

#include "rs_init_partition.h"

void rs_init_partition(rs_t *rs, rs_disk_size_t size, rs_environment_t env) {
    
    switch(size) {
        
        case RS_DISK_32K:
            if (env == RS_ENV_VIRTUAL) {
                rs->driver = RS_DRIVER_VIRTUAL;
            } else {
                rs->driver = RS_DRIVER_ARDUINO_EEPROM;
            }
            rs->memory_size = 0x7f94; //32660;
            rs->resource_descriptor_table_address = 0x0020; //32;
            rs->cluster_table_address = 0x00a0; //160;
            rs->sizeof_resource_descriptor_table = 0x0080; //128;
            rs->sizeof_cluster_table = 0x7ef4; //32500;
            rs->sizeof_resource_descriptor = 0x04; //4;
            rs->sizeof_cluster = 0x82; //130;
            rs->resource_descriptor_count = 0x20; //32;
            rs->cluster_count = 0xfa; //250;
            rs->sizeof_cluster_data = 0x80; //128;
            rs->sizeof_cluster_control = 0x02; //2;
            rs->free_clusters = 0xfa; //250;
            rs->flags = 0x00; //0;
        break;
        
        case RS_DISK_24K:
            if (env == RS_ENV_VIRTUAL) {
                rs->driver = RS_DRIVER_VIRTUAL;
            } else {
                rs->driver = RS_DRIVER_ARDUINO_EEPROM;
            }
            rs->memory_size = 0x5f96; //24470;
            rs->resource_descriptor_table_address = 0x20; //32;
            rs->cluster_table_address = 0xa0; //160;
            rs->sizeof_resource_descriptor_table = 0x80; //128;
            rs->sizeof_cluster_table = 0x5ef6; //24310;
            rs->sizeof_resource_descriptor = 0x4; //4;
            rs->sizeof_cluster = 0x82; //130;
            rs->resource_descriptor_count = 0x20; //32;
            rs->cluster_count = 0xbb; //187;
            rs->sizeof_cluster_data = 0x80; //128;
            rs->sizeof_cluster_control = 0x2; //2;
            rs->free_clusters = 0xbb; //187;
            rs->flags = 0x00; //0;
        break;

        case RS_DISK_8K:
            if (env == RS_ENV_VIRTUAL) {
                rs->driver = RS_DRIVER_VIRTUAL;
            } else {
                rs->driver = RS_DRIVER_ARDUINO_EEPROM;
            }
            rs->memory_size = 0x2000; //8192;
            rs->resource_descriptor_table_address = 0x0020; //32;
            rs->cluster_table_address = 0x00a0; //160;
            rs->sizeof_resource_descriptor_table = 0x0080; //128;
            rs->sizeof_cluster_table = 0x1f60; //8032;
            rs->sizeof_resource_descriptor = 0x04; //4;
            rs->sizeof_cluster = 0x20; //32;
            rs->resource_descriptor_count = 0x20; //32;
            rs->cluster_count = 0xfb; //251;
            rs->sizeof_cluster_data = 0x1e; //30;
            rs->sizeof_cluster_control = 0x02; //2;
            rs->free_clusters = 0xfb; //251;
            rs->flags = 0x00; //0;
        break;
        
        default:
            
            if (env == RS_ENV_VIRTUAL) {
                rs->driver = RS_DRIVER_VIRTUAL;
            } else {
                rs->driver = RS_DRIVER_ARDUINO_EEPROM;
            }
            rs->memory_size = 0xf46; //3910;
            rs->resource_descriptor_table_address = 0x0020; //32;
            rs->cluster_table_address = 0x00a0; //160;
            rs->sizeof_resource_descriptor_table = 0x0080; //128;
            rs->sizeof_cluster_table = 0xea6; //3750;
            rs->sizeof_resource_descriptor = 0x04; //4;
            rs->sizeof_cluster = 0x0f; //32;
            rs->resource_descriptor_count = 0x20; //32;
            rs->cluster_count = 0xfa; //250;
            rs->sizeof_cluster_data = 0x0d; //13;
            rs->sizeof_cluster_control = 0x02; //2;
            rs->free_clusters = 0xfa; //250;
            rs->flags = 0x00; //0;
        break;
    }
}

#endif // __SDCC_RS_INIT_PARTITION_C__
