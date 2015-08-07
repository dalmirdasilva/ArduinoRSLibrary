#include <Wire.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <ExternalMappedEeprom.h>

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
#include <SeekableInputStream.h>
#include <ResourceSeekableInputStream.h>

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
    sprintf("\n=\n");
    sprintf("\nRs\n");
    sprintf("--\n");
    sprintf("memory_size:           0x%04x %4d %s\n", rs->memory_size, rs->memory_size, itob(rs->memory_size));
    sprintf("res_desc_table_addr:   0x%04x %4d %s\n", rs->resource_descriptor_table_address, rs->resource_descriptor_table_address, itob(rs->resource_descriptor_table_address));
    sprintf("cluster_table_addr:    0x%04x %4d %s\n", rs->cluster_table_address, rs->cluster_table_address, itob(rs->cluster_table_address));
    sprintf("sizeof_res_desc_table: 0x%04x %4d %s\n", rs->sizeof_resource_descriptor_table, rs->sizeof_resource_descriptor_table, itob(rs->sizeof_resource_descriptor_table));
    sprintf("sizeof_cluster_table:  0x%04x %4d %s\n", rs->sizeof_cluster_table, rs->sizeof_cluster_table, itob(rs->sizeof_resource_descriptor_table));
    sprintf("sizeof_res_desc:       0x%04x %4d %s\n", rs->sizeof_resource_descriptor, rs->sizeof_resource_descriptor, itob(rs->sizeof_resource_descriptor));
    sprintf("sizeof_cluster:        0x%04x %4d %s\n", rs->sizeof_cluster, rs->sizeof_cluster, itob(rs->sizeof_cluster));
    sprintf("res_desc_cnt:          0x%04x %4d %s\n", rs->resource_descriptor_count, rs->resource_descriptor_count, itob(rs->resource_descriptor_count));
    sprintf("cluster_cnt:           0x%04x %4d %s\n", rs->cluster_count, rs->cluster_count, itob(rs->cluster_count));
    sprintf("sizeof_clus_data:      0x%04x %4d %s\n", rs->sizeof_cluster_data, rs->sizeof_cluster_data, itob(rs->sizeof_cluster_data));
    sprintf("sizeof_clus_ctrl:      0x%04x %4d %s\n", rs->sizeof_cluster_control, rs->sizeof_cluster_control, itob(rs->sizeof_cluster_control));
    sprintf("free_clus:             0x%04x %4d %s\n", rs->free_clusters, rs->free_clusters, itob(rs->free_clusters));
    sprintf("flags:                 0x%04x %4d %s\n", rs->flags, rs->flags, itob(rs->flags));
    sprintf("\n=\n");
    sprintf("\nRS table\n");
    sprintf("--\n");
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
    sprintf("\n==\n");
    sprintf("\nCluster table\n");
    sprintf("--\n");
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
    rs_init_partition(&rs, RS_DISK_24K, RS_ENV_ARDUINO);

    SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
    External24cl256Eeprom eeprom(0x00);
    ExternalMappedEeprom eme1(&eeprom, 8*1024, 32*1024 - 1);

    SimpleExternalEepromResourceIO io(&eme1);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);

    sprintf("m ");
    if (resourceSystem.mount(ResourceSystem::MOUNT_READ_ONLY)) {
        sprintf("ok\n");
    } else {
        sprintf("f, lor: %x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }
    
    rs_dump(resourceSystem.getRs());

    sprintf("u ");
    if (resourceSystem.umount()) {
        sprintf("ok\n");
    } else {
        sprintf("f, lor: %x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }
}

void loop() {
}