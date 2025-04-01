#include "main.h"
#include "eeprom.h"

#include "adau1761.h"


#define EEPROM_BASE_ADDR	0x08080000
#define EEPROM_BYTE_SIZE	0x03FF
#define iEEPROM_CHECK_NUM 2

//Byte write
static void EEPROM_WRITE(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint16_t i;
		HAL_StatusTypeDef status = HAL_OK;

		HAL_FLASHEx_DATAEEPROM_Unlock();
		for(i=0;i<len;i++)
		{
				status +=HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, EEPROM_BASE_ADDR+BiasAddress+i, *Data);
				Data++;
		}
		HAL_FLASHEx_DATAEEPROM_Lock();
}
//Byte read
static void EEPROM_READ(uint16_t BiasAddress,uint8_t *Buffer,uint16_t Len)
{
		uint8_t *wAddr;
		wAddr=(uint8_t *)(EEPROM_BASE_ADDR+BiasAddress);
		while(Len--)
				*Buffer++=*wAddr++;
}



HAL_StatusTypeDef EEPROM_WRITE_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint8_t buff[len];
		uint16_t i;
		for (i=0;i<iEEPROM_CHECK_NUM;i++)
		{
			EEPROM_WRITE(BiasAddress, Data, len);
			EEPROM_READ(BiasAddress, buff, len);
			if (memcmp(Data, buff, len)==0)
			{
				return HAL_OK;
			}
		}

		return HAL_ERROR;
}

HAL_StatusTypeDef EEPROM_Read_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint8_t buff0[len];
		uint8_t buff1[len];
		uint16_t i;
		for (i=0;i<iEEPROM_CHECK_NUM;i++)
		{
			EEPROM_READ(BiasAddress, buff0, len);
			EEPROM_READ(BiasAddress, buff1, len);

				if (memcmp(buff0, buff1, len)==0)
				{
						memcpy(Data, buff0, len);
						return HAL_OK;
				}
		}
		return HAL_ERROR;
}


//向eeprom保存eq信息
void bsp_write_eqs_to_eeprom(int8_t *eq_boost)
{
		for(uint8_t i=0; i<=9; i++)
		{
				EEPROM_WRITE_W_CHECK(i, (uint8_t *)&eq_boost[i], 1);
		}
}
//从eeprom加载eq信息
void bsp_read_eqs_from_eeprom(int8_t *eq_boost)
{
		for(uint8_t i=0; i<=9; i++)
		{
				EEPROM_Read_W_CHECK(i, (uint8_t *)&eq_boost[i], 1);
		}
}
