/**
 * Arduino - A simple resource implementation
 * 
 * SimpleVirtualResourceIO.cpp
 * 
 * This is the Resource IO representation.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_SIMPLE_VIRTUAL_RESOURCE_IO_CPP__
#define __ARDUINO_SIMPLE_VIRTUAL_RESOURCE_IO_CPP__ 1

#include <stdio.h>
#include <stddef.h>
#include "SimpleVirtualResourceIO.h"

SimpleVirtualResourceIO::SimpleVirtualResourceIO(char *fileName) : SimpleResourceIO(), fileName(fileName) {
    open();
}

bool SimpleVirtualResourceIO::open() {
    fp = fopen(fileName, "rb+");
    if (fp == NULL) {
        printf("Error when opening file: %s.\n", fileName);
        exit(1);
    }
    return true;
}

void SimpleVirtualResourceIO::flush() {
    SimpleResourceIO::flush();
    fflush(fp);
}

void SimpleVirtualResourceIO::close() {
    SimpleResourceIO::close();
    fclose(fp);
}

int SimpleVirtualResourceIO::readBytes(unsigned int address, unsigned char* buf, int len) {
    fseek(fp, address, 0);
    return (int) fread(buf, sizeof(unsigned char), len, fp);
}

void SimpleVirtualResourceIO::writeBytes(unsigned int address, unsigned char* buf, int len) {
    fseek(fp, address, 0);
    fwrite(buf, sizeof(unsigned char), len, fp);
}

#endif	/* __ARDUINO_SIMPLE_VIRTUAL_RESOURCE_IO_CPP__ */

