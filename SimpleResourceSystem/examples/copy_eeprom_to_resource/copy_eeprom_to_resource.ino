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
#include <Closeable.h>
#include <OutputStream.h>
#include <InputStream.h>
#include <ResourceOutputStream.h>
#include <ResourceInputStream.h>


#include <stdarg.h>

void sprintf(char *fmt, ...) {
    char tmp[64];
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, 64, fmt, args);
    va_end(args);
    Serial.print(tmp);
}

void setup() {
    Serial.begin(9600);
    SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
    External24cl256Eeprom resourceSystemEeprom(0x00);
    External24cl256Eeprom originEeprom(0x02);

    // the end 24k
    ExternalMappedEeprom mapped(&resourceSystemEeprom, 8 * 1024, 32 * 1024 - 1);
    SimpleExternalEepromResourceIO io(&mapped);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);

    sprintf("m ");
    if (resourceSystem.mount(ResourceSystem::MOUNT_READ_WRITE)) {
        sprintf("ok\n");
    } else {
        sprintf("f, lor: %x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }

    Serial.println("It will alloc new resource. (y?)");
    while (Serial.read() != 'y');

    SimpleResource rw = resourceSystem.alloc();
    sprintf("code: %x\n", rw.getCode());
    sprintf("open: %x\n", rw.open(Resource::OPEN_READ_WRITE));

    sprintf("writing...");
    ResourceOutputStream ros(&rw);
    for (long i = 0; i < 482; i++) {
        ros.write(originEeprom.read(i));
    }
    ros.close();
    sprintf("done.\n");
    
    sprintf("rw size: %d\n", rw.size());
    
    sprintf("open: %x\n", rw.open(Resource::OPEN_READ_ONLY));
    ResourceInputStream ris(&rw);
    while (ris.available()) {
        sprintf("%x\n", ris.read());
    }
    rw.close();
    
    sprintf("Umonting... ");
    if (resourceSystem.umount()) {
        sprintf("done.\n");
    } else {
        sprintf("fail. LOR: %x\n", (int) resourceSystem.getLastOperationResult());
        return;
    }
}

void loop() {
}
