// IO
#include <Wire.h>
#include <Closeable.h>
#include <Seekable.h>
#include <InputStream.h>
#include <SeekableInputStream.h>
#include <ResourceInputStream.h>
#include <ResourceSeekableInputStream.h>
#include <ExternalEepromInputStream.h> 
#include <ExternalEepromSeekableInputStream.h>

// Eeprom
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <ExternalMappedEeprom.h>

// Glcd
#include <Glcd.h>
#include <GlcdStraight.h>
#include <GlcdGraphicState.h>
#include <GlcdDrawer.h>
#include <GlcdShapes.h>
#include <GlcdText.h>
#include <GlcdSimpleText.h>
#include <GlcdBitmapFont.h>

// Resource
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
    Serial.println("Sure? (y/n)");
    while (Serial.read() != 'y');
    External24cl256Eeprom ee(0x00);
    
    ExternalMappedEeprom mapped(&ee, 8 * 1024, 32 * 1024 - 1);
    SimpleResourceSystem resourceSystem(RS_DRIVER_ARDUINO_EEPROM);
    SimpleExternalEepromResourceIO io(&mapped);
    SimpleResourceIO::associateIO(&io, RS_DRIVER_ARDUINO_EEPROM);
    
    if (resourceSystem.mount(ResourceSystem::MOUNT_READ_ONLY)) {
        Serial.println("monted.");
    } else {
        Serial.print("mount lor: ");
        Serial.println((int) resourceSystem.getLastOperationResult());
        return;
    }
    
    SimpleResource rw = resourceSystem.getResourceByCode(0);
    if (!rw.open(Resource::OPEN_READ_ONLY)) {
        sprintf("cannot open.");
        return;
    }
    
    GlcdGraphicState graphicState;
    GlcdStraight glcdDriver(A2, A3, A1, 0);

    glcdDriver.init(Glcd::MODE_ON);
    glcdDriver.screen(0x00);
    graphicState.setColor(Glcd::COLOR_WHITE);

    ResourceSeekableInputStream rsis(&rw);
    GlcdBitmapFont font(&rsis);
    GlcdText glcdText(&glcdDriver, &font, &graphicState);

    Rectangle rec(0, 0, 127, 63);
    glcdText.printString(&rec, (unsigned char*) "Lorem ipsum dolor sit amet, consectetur adipiscing elit, consectetur adipiscing elit, consectetur adipiscing elit..", 100, 1);
}

void loop() {
}