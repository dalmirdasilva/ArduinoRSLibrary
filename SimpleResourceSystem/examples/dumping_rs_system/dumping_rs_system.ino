#include <Wire.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <ExternalMappedEeprom.h>
#include <rs.h>
#include <rs_util.h>
#include <rs_io.h>
#include <Resource.h>
#include <ResourceIO.h>
#include <ResourceSystem.h>
#include <SimpleResource.h>
#include <SimpleResourceIO.h>
#include <SimpleResourceSystem.h>
#include <SimpleExternalEepromResourceIO.h>

void setup() {
    Serial.begin(9600);

    SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
    External24cl256Eeprom eeprom(0x00);
    ExternalMappedEeprom mapped(&eeprom, 8 * 1024, 32 * 1024 - 1);
    SimpleExternalEepromResourceIO io(&mapped);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);

    Serial.print("Monting... ");
    if (resourceSystem.mount(ResourceSystem::MOUNT_READ_ONLY)) {
        Serial.println("done.");
    } else {
        Serial.print("fail. Last operation result: ");
        Serial.println((int) resourceSystem.getLastOperationResult());
        return;
    }

    Serial.print("total space: ");
    Serial.println(resourceSystem.totalSpace());

    Serial.print("available space: ");
    Serial.println(resourceSystem.availableSpace());

    Serial.print("Umonting... ");
    if (resourceSystem.umount()) {
        Serial.println("done.");
    } else {
        Serial.print("fail. Last operation result: ");
        Serial.println((int) resourceSystem.getLastOperationResult());
        return;
    }
}

void loop() {
}
