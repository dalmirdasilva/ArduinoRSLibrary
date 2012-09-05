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

void setup() {
  Serial.begin(9600);

  rs_t rs;
  rs_init_partition(&rs, RS_DISK_24K, RS_ENV_ARDUINO);

  Serial.println("It will erase the eeprom. (y?)");
  while(Serial.read() != 'y');

  SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
  External24cl256Eeprom eeprom(0x00);
  
  // the end 24k
  ExternalMappedEeprom mapped(&eeprom, 8*1024, 32*1024-1);
  SimpleExternalEepromResourceIO io(&mapped);
  SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);

  Serial.print("Formating... ");
  if (SimpleResourceSystem::format(&rs)) {
    Serial.println("Done!");
  } else {
    Serial.println("Fail!");
  }
}

void loop() {
}
