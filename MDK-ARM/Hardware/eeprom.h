#ifndef   _EEPROM_H_
#define   _EEPROM_H_

HAL_StatusTypeDef EEPROM_WriteWithCheck(uint16_t BiasAddress, uint8_t *Data, uint16_t len);
HAL_StatusTypeDef EEPROM_ReadWithCheck(uint16_t BiasAddress, uint8_t *Data, uint16_t len);

#endif
