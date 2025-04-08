#include "main.h"
#include "eeprom.h"

#include "adau1761.h"


#define EEPROM_BASE_ADDR	0x08080000
#define EEPROM_BYTE_SIZE	0x03FF
#define iEEPROM_CHECK_NUM 2

// 向EEPROM写入数据，写入地址为EEPROM_BASE_ADDR+BiasAddress
static void EEPROM_Write(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint16_t i;
		HAL_StatusTypeDef status = HAL_OK;

		HAL_FLASHEx_DATAEEPROM_Unlock();
		for(i=0;i<len;i++)
		{
				status += HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, EEPROM_BASE_ADDR+BiasAddress+i, *Data);
				Data++;
		}
		HAL_FLASHEx_DATAEEPROM_Lock();
}
// 从EEPROM读出数据，读取地址为EEPROM_BASE_ADDR+BiasAddress
static void EEPROM_Read(uint16_t BiasAddress,uint8_t *Buffer,uint16_t Len)
{
		uint8_t *wAddr;
		wAddr=(uint8_t *)(EEPROM_BASE_ADDR+BiasAddress);
		while(Len--)
				*Buffer++=*wAddr++;
}


// 向EEPROM写入数据，写入地址为EEPROM_BASE_ADDR+BiasAddress
// 写入后进行读取、比较校验
HAL_StatusTypeDef EEPROM_WriteWithCheck(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint8_t buff[len];
		uint16_t i;
		for (i=0;i<iEEPROM_CHECK_NUM;i++)
		{
			EEPROM_Write(BiasAddress, Data, len);
			EEPROM_Read(BiasAddress, buff, len);
			if (memcmp(Data, buff, len)==0)
			{
				return HAL_OK;
			}
		}
		return HAL_ERROR;
}
// 从EEPROM读取数据，读出地址为EEPROM_BASE_ADDR+BiasAddress
// 读取两次进行比较校验
HAL_StatusTypeDef EEPROM_ReadWithCheck(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
		uint8_t buff0[len];
		uint8_t buff1[len];
		uint16_t i;
		for (i=0;i<iEEPROM_CHECK_NUM;i++)
		{
			EEPROM_Read(BiasAddress, buff0, len);
			EEPROM_Read(BiasAddress, buff1, len);

				if (memcmp(buff0, buff1, len)==0)
				{
						memcpy(Data, buff0, len);
						return HAL_OK;
				}
		}
		return HAL_ERROR;
}
