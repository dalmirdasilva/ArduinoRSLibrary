/**
 * SDCC - PIC resource system
 * 
 * rs_io.c
 * 
 * IO lib for rs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_IO_C__
#define __SDCC_RS_IO_C__ 1

#include "rs_io.h"

uint8_t _rs_io_read(rs_driver_t driver, rs_memory_address_t address) {
    return SimpleResourceIO::getAssociatedIO(driver)->read(address);
}

void _rs_io_write(rs_driver_t driver, rs_memory_address_t address, uint8_t data) {
    SimpleResourceIO::getAssociatedIO(driver)->write(address, data);
}

#endif // __SDCC_RS_IO_C__
