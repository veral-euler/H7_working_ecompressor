#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

#define EEPROM_ADDRESS 0xA0 // Adjust based on your EEPROM's I2C address

uint8_t EEPROM_Write(uint16_t addr, uint8_t *data, uint16_t size);
uint8_t EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t size);

#endif