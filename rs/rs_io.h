/**
 * SDCC - PIC resource system
 * 
 * rs_io.h
 * 
 * IO lib for rs
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __SDCC_RS_IO_H__
#define __SDCC_RS_IO_H__ 1

#include "rs.h"
#include <stdint.h>
#include <SimpleResourceIO.h>

uint8_t _rs_io_read(rs_driver_t driver, rs_memory_address_t address);

void _rs_io_write(rs_driver_t driver, rs_memory_address_t address, uint8_t data);

#endif // __SDCC_RS_IO_H__
