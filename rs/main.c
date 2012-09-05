#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "rs_io.c"
#include "rs_util.c"
#include "rs.c"
#include "rs_init_partition.c"

void resource_dump(rs_resource_t *resource);

#include "rs_spec.h"

void format_all() {
    uint16_t i;
    for (i = 0; i < 0x7fff; i++) {
        _rs_io_write(RS_DRIVER_VIRTUAL, i, 0x00);
    }
}

int main() {
    rs_t rs;

    format_all();
    rs_init_partition(&rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    rs_format(&rs);
    format_spec(&rs);
    mount_spec(&rs);
    umount_spec(&rs);
    alloc_resource_spec(&rs);
    try_to_alloc_resources_that_is_possible_spec(&rs);
    open_resource_spec(&rs);
    write_resource_spec(&rs);
    rewind_resource_spec(&rs);
    read_resource_spec(&rs);
    close_resource_spec(&rs);
    try_read_when_end_of_resource_is_reached_spec(&rs);
    try_read_when_resource_is_closed_spec(&rs);
    seek_resource_spec(&rs);
    random_read_resource_spec(&rs);
    random_read_with_seek_resource_spec(&rs);
    random_read_with_seek_opening_resource_spec(&rs);
    size_resource_spec(&rs);
    tell_resource_spec(&rs);
    tell_with_seek_resource_spec(&rs);
    total_space_resource_spec(&rs);
    allocating_multi_format_spec(&rs);
    read_only_mounting_spec(&rs);
    read_only_opening_spec(&rs);

    rs_mount(RS_SPEC_DRIVER, &rs, RS_MOUNT_OPTION_NORMAL);
    _rs_io_memory_dump(&rs);
    return 0;
}

void resource_dump(rs_resource_t *resource) {
    printf("======== resource dump begin ========\n");
    printf("  resource descriptor: %6d %s\n", resource->resource_descriptor, itob(resource->resource_descriptor));
    printf("  first cluster:______ %6d %s\n", resource->first_cluster, itob(resource->first_cluster));
    printf("  current cluster:____ %6d %s\n", resource->current_cluster, itob(resource->current_cluster));
    printf("  cluster offset:_____ %6d %s\n", resource->cluster_offset, itob(resource->cluster_offset));
    printf("  size:_______________ %6d %s\n", resource->size, itob(resource->size));
    printf("  current position:___ %6d %s\n", resource->current_position, itob(resource->current_position));
    printf("  flags:______________ %6d %s\n", resource->flags, itob(resource->flags));
    printf("  errors:_____________ %6d %s\n", rs_error(resource), itob(rs_error(resource)));
    printf("========= resource dump end =========\n");
}
