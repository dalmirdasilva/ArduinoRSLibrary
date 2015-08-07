#define VIRTUAL_ENVIROMENT 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rs.h>
#include <rs_util.h>
#include <rs_io.c>
#include <rs_util.c>
#include <rs.c>
#include <rs_init_partition.c>

#include <Seekable.h>
#include <Seekable.cpp>
#include <Closeable.h>
#include <Closeable.cpp>
#include <InputStream.h>
#include <InputStream.cpp>
#include <ResourceInputStream.h>
#include <ResourceInputStream.cpp>

#include <OutputStream.h>
#include <OutputStream.cpp>
#include <ResourceOutputStream.h>
#include <ResourceOutputStream.cpp>

#include <SimpleResource.h>
#include <SimpleResourceSystem.h>
#include <SimpleResource.cpp>
#include <SimpleResourceSystem.cpp>
#include <SimpleResourceIO.h>
#include <SimpleResourceIO.cpp>
#include <SimpleVirtualResourceIO.h>
#include <SimpleVirtualResourceIO.cpp>
#include <SimpleExternalEepromResourceIO.h>
#include <SimpleExternalEepromResourceIO.cpp>
#include <SimpleArrayResourceIO.h>
#include <SimpleArrayResourceIO.cpp>
#include <ExternalEeprom.h>
#include <ExternalEeprom.cpp>
#include <ExternalByteArrayEeprom.h>
#include <ExternalByteArrayEeprom.cpp>

char* itob(int i) {
    int bits;
    int j, k;
    uint16_t mi = 0;
    mi |= i;
    static char buff[sizeof (mi) * 8 + 1];
    bits = sizeof (mi) * 8;
    for (j = bits - 1, k = 0; j >= 0; j--, k++) {
        buff[k] = ((mi >> j) & 0x01) + '0';
    }
    buff[bits] = '\0';
    return buff;
}

void _rs_io_memory_dump(rs_t *rs) {
    rs_memory_address_t memory_address;
    uint16_t count, count2;
    uint8_t d = 0;
    FILE *fp;
    if (!_rs_is_driver_monted(rs->driver)) {
        printf("Rs not mouted yet\n");
        return;
    }
    fp = fopen("dump", "w+");
    fprintf(fp, "DRIVER: %x\n", rs->driver);
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nRs\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "memory_size:                       0x%04x %4d %s\n", rs->memory_size, rs->memory_size, itob(rs->memory_size));
    fprintf(fp, "resource_descriptor_table_address: 0x%04x %4d %s\n", rs->resource_descriptor_table_address, rs->resource_descriptor_table_address, itob(rs->resource_descriptor_table_address));
    fprintf(fp, "cluster_table_address:             0x%04x %4d %s\n", rs->cluster_table_address, rs->cluster_table_address, itob(rs->cluster_table_address));
    fprintf(fp, "sizeof_resource_descriptor_table:  0x%04x %4d %s\n", rs->sizeof_resource_descriptor_table, rs->sizeof_resource_descriptor_table, itob(rs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_cluster_table:              0x%04x %4d %s\n", rs->sizeof_cluster_table, rs->sizeof_cluster_table, itob(rs->sizeof_resource_descriptor_table));
    fprintf(fp, "sizeof_resource_descriptor:        0x%04x %4d %s\n", rs->sizeof_resource_descriptor, rs->sizeof_resource_descriptor, itob(rs->sizeof_resource_descriptor));
    fprintf(fp, "sizeof_cluster:                    0x%04x %4d %s\n", rs->sizeof_cluster, rs->sizeof_cluster, itob(rs->sizeof_cluster));
    fprintf(fp, "resource_descriptor_count:         0x%04x %4d %s\n", rs->resource_descriptor_count, rs->resource_descriptor_count, itob(rs->resource_descriptor_count));
    fprintf(fp, "cluster_count:                     0x%04x %4d %s\n", rs->cluster_count, rs->cluster_count, itob(rs->cluster_count));
    fprintf(fp, "sizeof_cluster_data:               0x%04x %4d %s\n", rs->sizeof_cluster_data, rs->sizeof_cluster_data, itob(rs->sizeof_cluster_data));
    fprintf(fp, "sizeof_cluster_control:            0x%04x %4d %s\n", rs->sizeof_cluster_control, rs->sizeof_cluster_control, itob(rs->sizeof_cluster_control));
    fprintf(fp, "free_clusters:                     0x%04x %4d %s\n", rs->free_clusters, rs->free_clusters, itob(rs->free_clusters));
    fprintf(fp, "flags:                             0x%04x %4d %s\n", rs->flags, rs->flags, itob(rs->flags));
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nResource table\n");
    fprintf(fp, "-----------------\n");
    count = 0;
    for (
        memory_address = rs->resource_descriptor_table_address;
        memory_address < (rs->resource_descriptor_table_address + rs->sizeof_resource_descriptor_table);
        memory_address++
        ) {

        if ((count % rs->sizeof_resource_descriptor) == 0) {
            fprintf(fp, "\n%02x: ", (count) ? count / rs->sizeof_resource_descriptor : 0);
        }
        fprintf(fp, "%02x ", _rs_io_read(rs->driver, memory_address));
        count++;
    }
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "\nCluster table\n");
    fprintf(fp, "-----------------\n");
    fprintf(fp, "\n    |nn |pp |");
    for (count = 0; count < rs->sizeof_cluster_data; count++) {
        fprintf(fp, "dd ");
    }
    fprintf(fp, "\n    ---------");
    for (count = 0; count < rs->sizeof_cluster_data; count++) {
        fprintf(fp, "---");
    }
    count = 0;

    for (
        memory_address = rs->cluster_table_address;
        memory_address < (rs->cluster_table_address + rs->sizeof_cluster_table);
        memory_address++
        ) {
        if ((count % rs->sizeof_cluster) == 0) {
            fprintf(fp, "\n%02x: |", (count) ? count / rs->sizeof_cluster : 0);
            count2 = 0;
        }
        if (count2 == 1 || count2 == 2) {
            fprintf(fp, "|");
        }
        fprintf(fp, "%02x ", (d = _rs_io_read(rs->driver, memory_address)));
        fflush(fp);
        count++;
        count2++;
    }
    fclose(fp);
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

void wrapper_format(rs_t *rs) {
    uint8_t b[5] = {0xf0, 0x01, 0xff, 0xdd, 0xfa};
    uint8_t a[32768];
    uint8_t c[32768];
    rs->driver = RS_DRIVER_VIRTUAL;
    printf("format: %x\n", SimpleResourceSystem::format(rs));
    SimpleResourceSystem rsw(RS_DRIVER_VIRTUAL);
    printf("mount: %x\n", rsw.mount(ResourceSystem::MOUNT_READ_WRITE));
    SimpleResource rw = rsw.alloc();
    printf("code: %x\n", rw.getCode());
    printf("open: %d\n", rw.open(Resource::OPEN_READ_WRITE));
    rw.writeBytes(b, 5);
    rw.rewind();
    printf("read: %x\n", rw.read());
    printf("size: %x\n", rw.size());
    rw.seek(Resource::SEEK_ORIGIN_BEGIN, 2);
    printf("read: %x\n", rw.read());
    printf("availableSpace: %x\n", rsw.availableSpace());
    rw.release();
    printf("availableSpace: %x\n", rsw.availableSpace());
    printf("close: %x\n", rw.close());
    printf("open: %d\n", rw.open(Resource::OPEN_READ_WRITE));
    printf("lor: %d\n", rw.getLastOperationResult());
}

int main() {
    rs_t rs;
    char *s = (char *) "/tmp/img.bin";
    SimpleVirtualResourceIO io(s);
    
    printf("Running wrapper specs...\n");
    
    rs_init_partition(&rs, RS_DISK_32K, RS_ENV_VIRTUAL);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_VIRTUAL);
    
    //wrapper_format(&rs);

    printf("Format: %d\n", SimpleResourceSystem::format(&rs));
    SimpleResourceSystem srs(RS_DRIVER_VIRTUAL);
    printf("Mount: %d\n", srs.mount(ResourceSystem::MOUNT_READ_WRITE));
    SimpleResource rw = srs.alloc();
    printf("code: %x\n", rw.getCode());
    rw.open(Resource::OPEN_READ_WRITE);

    ResourceOutputStream ros(&rw);
    for (int i = 0; i < 250; i++) {
        ros.write(i);
    }
    printf("rw size: %d\n", rw.size());
    ros.close();
    
    rw.open(Resource::OPEN_READ_ONLY);
    ResourceInputStream ris(&rw);
    while (ris.available()) {
        ris.read();
    }
    
    rw.close();
    _rs_io_memory_dump(srs.getRs());
    return 0;
}
