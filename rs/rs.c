/**
 * SDCC - PIC resource system
 *      
 * rs.c
 * 
 * A file system implementation based on the idea of resources
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_C__
#define __SDCC_RS_C__ 1

#include "rs.h"
#include "rs_io.h"
#include "rs_util.h"
#include <stdint.h>

rs_global_flags_t rs_global_flags;

rs_op_result_t rs_format(rs_t *rs) {
    uint8_t i;
    _rs_write_rs_to_disc(rs->driver, rs);
    for (i = 0; i < rs->resource_descriptor_count; i++) {
        _rs_format_resorce_descriptor(rs, i);
    }
    for (i = 0; i < rs->cluster_count; i++) {
        _rs_format_cluster(rs, i);
    }
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_mount(rs_driver_t driver, rs_t *rs, rs_mount_options_t options) {
    if (_rs_is_driver_monted(driver)) {
        return RS_OP_RESULT_ERROR_DRIVER_BUSY;
    }
    _rs_read_rs_from_disc(driver, rs);
    _rs_set_driver_monted(driver, 1);
    if (options & RS_MOUNT_OPTION_READ_ONLY) {
        rs->flags |= RS_FLAG_BIT_READ_ONLY;
    }
    rs->driver = driver;
    _rs_free_resource_descriptors(rs);
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_umount(rs_t *rs) {
    if (_rs_is_driver_monted(rs->driver)) {
        _rs_set_driver_monted(rs->driver, 0);
    }
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_open(rs_t *rs, rs_resource_code_t resource_code, rs_resource_t *resource, rs_open_resource_options_t options) {
    uint8_t i;
    rs_memory_address_t address;
    rs_resource_descriptor_t resource_descriptor;
    uint8_t flags;
    if (!_rs_is_driver_monted(rs->driver)) {
        return RS_OP_RESULT_ERROR_DRIVER_NOT_MOUNTED;
    }
    resource_descriptor = _rs_resource_code_to_resource_descriptor(resource_code);
    address = _rs_resource_descriptor_to_address(rs, resource_descriptor);
    flags = _rs_io_read(rs->driver, RD_ADDRESS_TO_FLAG(address));
    if (!(flags & RS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return RS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (flags & RS_RESOURCE_FLAG_BIT_OPENED) {
        return RS_OP_RESULT_ERROR_RESOURCE_OPENED;
    }
    flags |= RS_RESOURCE_FLAG_BIT_OPENED;
    if ((options & RS_OPEN_RESOURCE_OPTION_READ_ONLY) || (rs->flags & RS_FLAG_BIT_READ_ONLY)) {
        flags |= RS_RESOURCE_FLAG_BIT_READ_ONLY;

    }
    _rs_io_write(rs->driver, RD_ADDRESS_TO_FLAG(address), flags);
    resource->resource_descriptor = resource_descriptor;
    resource->first_cluster = _rs_io_read(rs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(address));
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = rs->sizeof_cluster_control;
    resource->current_position = 0;
    for (i = 0; i < RS_SIZEOF_RESOURCE_SIZE; i++) {
        *((uint8_t *) (&resource->size) + i) = _rs_io_read(rs->driver, address + i);
    }
    resource->flags = flags;
    _rs_check_for_eor_reached(resource);
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_close(rs_t *rs, rs_resource_t *resource) {
    rs_sync(rs, resource);
    _rs_free_resource_descriptor(rs, resource->resource_descriptor);
    resource->flags = ~RS_RESOURCE_FLAG_BIT_OPENED;
    return RS_OP_RESULT_SUCCESS;
}

uint8_t rs_read(rs_t *rs, rs_resource_t *resource) {
    rs_memory_address_t address;
    uint8_t read_data;
    if (!(resource->flags & RS_RESOURCE_FLAG_BIT_OPENED)) {
        resource->flags |= RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ;
        return 0;
    }
    if (_rs_is_eor_reached(resource)) {
        return 0;
    }
    _rs_check_for_availability(rs, resource);
    address = _rs_cluster_to_address(rs, resource->current_cluster);
    read_data = _rs_io_read(rs->driver, address + resource->cluster_offset);
    resource->current_position++;
    resource->cluster_offset++;
    _rs_check_for_eor_reached(resource);
    return read_data;
}

rs_op_result_t rs_write(rs_t *rs, rs_resource_t *resource, uint8_t data_to_write) {
    rs_memory_address_t address;
    if (!(resource->flags & RS_RESOURCE_FLAG_BIT_OPENED)) {
        return RS_OP_RESULT_ERROR_RESOURCE_CLOSED;
    }
    if (resource->flags & RS_RESOURCE_FLAG_BIT_READ_ONLY) {
        return RS_OP_RESULT_ERROR_RESOURCE_READ_ONLY;
    }
    if (!_rs_check_for_availability(rs, resource)) {
        return RS_OP_RESULT_ERROR_NO_SPACE_AVAILABLE;
    }
    address = _rs_cluster_to_address(rs, resource->current_cluster);
    _rs_io_write(rs->driver, address + resource->cluster_offset, data_to_write);
    resource->cluster_offset++;
    resource->current_position++;
    if (rs_eor(resource)) {
        resource->size++;
        
        /*
         * It is causing seriously performance problems. Since the IO has a 
         * buffer, and we are writing at this buffer, once we need to sync, this
         * buffer will be flushed to the resource and the IO pointer will need
         * to go to the resource descriptor to write the new size of the 
         * resource. After the sync, we will continue to write the next byte,
         * the buffer filled on the sync will be lost and a new buffer will be
         * created to write the next byte... and so on.
         */
        //rs_sync(rs, resource);
    }
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_seek(rs_t *rs, rs_resource_t *resource, rs_seek_origin_t origin, rs_seek_int_t offset) {
    int16_t new_position = 0;
    if (resource->size == 0) {
        return RS_OP_RESULT_SUCCESS;
    }
    switch (origin) {
        case RS_SEEK_ORIGIN_BEGIN:
            new_position = offset;
            break;
        case RS_SEEK_ORIGIN_CURRENT:
            new_position = resource->current_position + offset;
            break;
    }
    new_position %= resource->size + 1;
    if (new_position < 0) {
        new_position += resource->size;
    }
    if (new_position == 0) {
        rs_rewind(rs, resource);
        return RS_OP_RESULT_SUCCESS;
    }
    if (new_position < resource->current_position) {
        if (new_position > (resource->current_position - new_position)) {
            _rs_move_current_position_back(rs, resource, (resource->current_position - new_position));
        } else {
            rs_rewind(rs, resource);
            _rs_move_current_position_ahead(rs, resource, new_position);
        }
    } else {
        _rs_move_current_position_ahead(rs, resource, (new_position - resource->current_position));
    }
    _rs_check_for_eor_reached(resource);
    return RS_OP_RESULT_SUCCESS;
}

rs_op_result_t rs_truncate(rs_t *rs, rs_resource_t *resource) {
    uint8_t flags;
    rs_memory_address_t resource_descriptor_address;
    uint8_t freed_clusters = 0;
    resource_descriptor_address = _rs_resource_descriptor_to_address(rs, resource->resource_descriptor);
    flags = _rs_io_read(rs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & RS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return RS_OP_RESULT_ERROR_RESOURCE_DOES_NOT_ALLOCATED;
    }
    if (resource->size > rs->sizeof_cluster_data) {
        freed_clusters = _rs_format_clusters_chain(rs, _rs_next_cluster_by_cluster(rs, resource->first_cluster));
    }
    _rs_increase_free_clusters(rs, freed_clusters);
    resource->size = 0x00;
    _rs_io_write(rs->driver, RD_ADDRESS_TO_SIZE_LOW(resource_descriptor_address), 0x00);
    _rs_io_write(rs->driver, RD_ADDRESS_TO_SIZE_HIGH(resource_descriptor_address), 0x00);
    return RS_OP_RESULT_SUCCESS;
}

void rs_sync(rs_t *rs, rs_resource_t *resource) {
    uint8_t i;
    rs_memory_address_t address;
    address = _rs_resource_descriptor_to_address(rs, resource->resource_descriptor);
    for (i = 0; i < 2; i++) {
        _rs_io_write(rs->driver, address + i, *((uint8_t *) (&(resource->size)) + i));
    }
}

void rs_stat(rs_t *rs, rs_resource_t *resource, rs_stat_t *stat) { // TODO
    stat->flags = 0xff;
}

rs_op_result_t rs_rewind(rs_t *rs, rs_resource_t *resource) {
    resource->current_cluster = resource->first_cluster;
    resource->cluster_offset = rs->sizeof_cluster_control;
    resource->current_position = 0;
    _rs_check_for_eor_reached(resource);
    return RS_OP_RESULT_SUCCESS;
}

rs_resource_code_t rs_alloc(rs_t *rs) {
    uint8_t i;
    uint8_t flags;
    rs_cluster_t first_cluster;
    rs_memory_address_t resource_descriptor_address, cluster_address;
    if (rs->free_clusters < 1) {
        return RS_NULL_RESOURCE_CODE;
    }
    resource_descriptor_address = rs->resource_descriptor_table_address;
    for (i = 0; i < rs->resource_descriptor_count; i++) {
        flags = _rs_io_read(rs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
        if (!(flags & RS_RESOURCE_FLAG_BIT_ALLOCATED)) {
            cluster_address = _rs_alloc_cluster(rs);
            if (cluster_address == RS_NULL_CLUSTER_ADDRESS) {
                return RS_NULL_RESOURCE_CODE;
            }
            flags |= RS_RESOURCE_FLAG_BIT_ALLOCATED;
            first_cluster = _rs_address_to_cluster(rs, cluster_address);
            _rs_create_cluster_chain(rs, first_cluster, RS_INEXISTENT_CLUSTER);
            _rs_io_write(rs->driver, RD_ADDRESS_TO_FIRST_CLUSTER(resource_descriptor_address), first_cluster);
            _rs_io_write(rs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address), flags);
            return _rs_resource_descriptor_to_resource_code(i);
        }
        resource_descriptor_address += rs->sizeof_resource_descriptor;
    }
    return RS_NULL_RESOURCE_CODE;
}

uint8_t rs_release(rs_t *rs, rs_resource_t *resource) {
    uint8_t flags;
    rs_memory_address_t resource_descriptor_address;
    resource_descriptor_address = _rs_resource_descriptor_to_address(rs, resource->resource_descriptor);
    flags = _rs_io_read(rs->driver, RD_ADDRESS_TO_FLAG(resource_descriptor_address));
    if (!(flags & RS_RESOURCE_FLAG_BIT_ALLOCATED)) {
        return 1;
    }
    _rs_format_resource_clusters(rs, resource);
    _rs_format_resorce_descriptor(rs, resource->resource_descriptor);
    resource->flags = 0x00;
    return 1;
}

rs_resource_size_t rs_size(rs_resource_t *resource) {
    return resource->size;
}

rs_resource_size_t rs_tell(rs_resource_t *resource) {
    return resource->current_position;
}

uint8_t rs_eor(rs_resource_t *resource) {
    return _rs_is_eor_reached(resource);
}

uint8_t rs_error(rs_resource_t *resource) {
    return (resource->flags & RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_READ || resource->flags & RS_RESOURCE_FLAG_BIT_ERROR_ON_LAST_WRITE);
}

rs_resource_size_t rs_available_space(rs_t *rs) {
    return rs->free_clusters * rs->sizeof_cluster_data;
}

rs_resource_size_t rs_total_space(rs_t *rs) {
    return rs->cluster_count * rs->sizeof_cluster_data;
}

#endif // __SDCC_RS_C__
