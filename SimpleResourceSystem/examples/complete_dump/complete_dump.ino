#include <Wire.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <rs.h>
#include <rs_util.h>
#include <rs_io.h>
#include <rs_init_partition.h>
#include <Resource.h>
#include <ResourceIO.h>
#include <ResourceSystem.h>
#include <SimpleResource.h>
#include <SimpleResourceIO.h>
#include <SimpleResourceSystem.h>
#include <SimpleExternalEepromResourceIO.h>

#include <Seekable.h>
#include <Closeable.h>
#include <InputStream.h>
#include <ResourceInputStream.h>

#include <OutputStream.h>
#include <ResourceOutputStream.h>

#include <FilterOutputStream.h>
#include <BufferedOutputStream.h>

#include <Seekable.h>
#include <Closeable.h>
#include <InputStream.h>
#include <ResourceInputStream.h>

#include <OutputStream.h>
#include <ResourceOutputStream.h>

#include <stdarg.h>

void sprintf(char *fmt, ...) {
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 128, fmt, args);
    va_end(args);
    Serial.print(tmp);
}

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

void rs_dump(rs_t *rs) {
    rs_memory_address_t memory_address;
    uint16_t count, count2;
    uint8_t d = 0;
    if (!_rs_is_driver_monted(rs->driver)) {
        sprintf("Rs not mouted yet\n");
        return;
    }
    sprintf("DRIVER: %x\n", rs->driver);
    sprintf("\n==========================\n");
    sprintf("\nRs\n");
    sprintf("-----------------\n");
    sprintf("memory_size:                       0x%04x %4d %s\n", rs->memory_size, rs->memory_size, itob(rs->memory_size));
    sprintf("resource_descriptor_table_address: 0x%04x %4d %s\n", rs->resource_descriptor_table_address, rs->resource_descriptor_table_address, itob(rs->resource_descriptor_table_address));
    sprintf("cluster_table_address:             0x%04x %4d %s\n", rs->cluster_table_address, rs->cluster_table_address, itob(rs->cluster_table_address));
    sprintf("sizeof_resource_descriptor_table:  0x%04x %4d %s\n", rs->sizeof_resource_descriptor_table, rs->sizeof_resource_descriptor_table, itob(rs->sizeof_resource_descriptor_table));
    sprintf("sizeof_cluster_table:              0x%04x %4d %s\n", rs->sizeof_cluster_table, rs->sizeof_cluster_table, itob(rs->sizeof_resource_descriptor_table));
    sprintf("sizeof_resource_descriptor:        0x%04x %4d %s\n", rs->sizeof_resource_descriptor, rs->sizeof_resource_descriptor, itob(rs->sizeof_resource_descriptor));
    sprintf("sizeof_cluster:                    0x%04x %4d %s\n", rs->sizeof_cluster, rs->sizeof_cluster, itob(rs->sizeof_cluster));
    sprintf("resource_descriptor_count:         0x%04x %4d %s\n", rs->resource_descriptor_count, rs->resource_descriptor_count, itob(rs->resource_descriptor_count));
    sprintf("cluster_count:                     0x%04x %4d %s\n", rs->cluster_count, rs->cluster_count, itob(rs->cluster_count));
    sprintf("sizeof_cluster_data:               0x%04x %4d %s\n", rs->sizeof_cluster_data, rs->sizeof_cluster_data, itob(rs->sizeof_cluster_data));
    sprintf("sizeof_cluster_control:            0x%04x %4d %s\n", rs->sizeof_cluster_control, rs->sizeof_cluster_control, itob(rs->sizeof_cluster_control));
    sprintf("free_clusters:                     0x%04x %4d %s\n", rs->free_clusters, rs->free_clusters, itob(rs->free_clusters));
    sprintf("flags:                             0x%04x %4d %s\n", rs->flags, rs->flags, itob(rs->flags));
    sprintf("\n==========================\n");
    sprintf("\nResource table\n");
    sprintf("-----------------\n");
    count = 0;
    for (
            memory_address = rs->resource_descriptor_table_address;
            memory_address < (rs->resource_descriptor_table_address + rs->sizeof_resource_descriptor_table);
            memory_address++
            ) {

        if ((count % rs->sizeof_resource_descriptor) == 0) {
            sprintf("\n%02x: ", (count) ? count / rs->sizeof_resource_descriptor : 0);
        }
        sprintf("%02x ", _rs_io_read(rs->driver, memory_address));
        count++;
    }
    sprintf("\n==========================\n");
    sprintf("\nCluster table\n");
    sprintf("-----------------\n");
    sprintf("\n    |nn |pp |");
    for (count = 0; count < rs->sizeof_cluster_data; count++) {
        sprintf("dd ");
    }
    sprintf("\n    ---------");
    for (count = 0; count < rs->sizeof_cluster_data; count++) {
        sprintf("---");
    }
    count = 0;

    for (
            memory_address = rs->cluster_table_address;
            memory_address < (rs->cluster_table_address + rs->sizeof_cluster_table);
            memory_address++
            ) {
        if ((count % rs->sizeof_cluster) == 0) {
            sprintf("\n%02x: |", (count) ? count / rs->sizeof_cluster : 0);
            count2 = 0;
        }
        if (count2 == 1 || count2 == 2) {
            sprintf("|");
        }
        sprintf("%02x ", (d = _rs_io_read(rs->driver, memory_address)));
        count++;
        count2++;
    }
}

void setup() {
    Serial.begin(9600);
    
    rs_t rs;
    rs_init_partition(&rs, RS_DISK_32K, RS_ENV_ARDUINO);

    SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
    External24cl256Eeprom eeprom(0x00);
    SimpleExternalEepromResourceIO io(&eeprom);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);

    sprintf("Format: %d\n", (int)SimpleResourceSystem::format(&rs));
    
    sprintf("Monting... ");
    if (resourceSystem.mount(ResourceSystem::MOUNT_READ_WRITE)) {
        sprintf("done.\n");
    } else {
        sprintf("fail. Last operation result: ");
        sprintf("%x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }
    
    SimpleResource rw = resourceSystem.alloc();
    sprintf("code: %x\n", rw.getCode());
    sprintf("open: %x\n", rw.open(Resource::OPEN_READ_WRITE));

    ResourceOutputStream ros(&rw);
    unsigned char b[16];
    BufferedOutputStream bos(&ros, b, 16);
    for (long i = 0; i < 1024; i++) {
        ros.write(i);
    }
    ros.close();
    
    sprintf("rw size: %d\n", rw.size());
    
    sprintf("open: %x\n", rw.open(Resource::OPEN_READ_ONLY));
    ResourceInputStream ris(&rw);
    while (ris.available()) {
        sprintf("%x\n", ris.read());
    }
    
    rw.close();
    
    rs_dump(resourceSystem.getRs());

    sprintf("Umonting... ");
    if (resourceSystem.umount()) {
        sprintf("done.\n");
    } else {
        sprintf("fail. Last operation result: ");
        sprintf("%x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }
}

void loop() {
}
