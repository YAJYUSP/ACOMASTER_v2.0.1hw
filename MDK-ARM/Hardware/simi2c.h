#ifndef   _SIMI2C_H_
#define  _SIMI2C_H_

#define IIC_WR	0		/* Ð´¿ØÖÆbit */
#define IIC_RD	  1		/* ¶Á¿ØÖÆbit */

#define IIC_SCL_1()  		HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)		    /* SCL = 1 */
#define IIC_SCL_0()    	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)		/* SCL = 0 */

#define IIC_SDA_1()  		HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)	  	/* SDA = 1 */
#define IIC_SDA_0()  		HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)  /* SDA = 0 */

#define IIC_SDA_READ()  HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin)								/* ¶ÁSDA¿ÚÏß×´Ì¬ */
#define IIC_Delay()     bsp_tim22_delay_us(2)


#include "stdint.h"


void IIC_NAck(void);
void IIC_Ack(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Stop(void);
void IIC_Start(void);

uint8_t IIC_Read_Byte(uint8_t ack);
void IIC_Send_Byte(uint8_t _ucByte);

uint8_t IIC_CheckDevice(uint8_t _Address);
uint8_t bsp_simi2c_write_len(uint8_t dev_addr, uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t bsp_simi2c_read_len(uint8_t dev_addr, uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t bsp_simi2c_write_byte(uint8_t dev_addr, uint8_t reg,uint8_t data);
uint8_t bsp_simi2c_read_byte(uint8_t dev_addr, uint8_t reg);


#endif
