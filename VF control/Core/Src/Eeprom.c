#include "Eeprom.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t EEPROM_Write(uint16_t addr, uint8_t *data, uint16_t size)
{
  if (HAL_I2C_Mem_Write(&hi2c2, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY) == HAL_OK)
  {
    return 0; // Success
  }
  return 1; // Error
}

uint8_t EEPROM_Read(uint16_t addr, uint8_t *data, uint16_t size)
{
  if (HAL_I2C_Mem_Read(&hi2c2, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY) == HAL_OK)
  {
    return 0; // Success
  }
  return 1; // Error
}
