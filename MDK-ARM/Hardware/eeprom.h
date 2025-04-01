#ifndef   _EEPROM_H_
#define   _EEPROM_H_

HAL_StatusTypeDef EEPROM_WRITE_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len);
HAL_StatusTypeDef EEPROM_Read_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len);

void bsp_write_eqs_to_eeprom(int8_t *eq_boost);
void bsp_read_eqs_from_eeprom(int8_t *eq_boost);

#endif
