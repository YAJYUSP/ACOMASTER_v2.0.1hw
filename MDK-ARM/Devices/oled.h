#ifndef __OLED_H
#define __OLED_H			  	 

#include <stdlib.h>
#include <main.h>
#include <stdint.h>

#define PI 3.14159F

#define u8 uint8_t
#define OLED_SLAVE_ADDR 0x78

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

#define OLED_PIXEL_X 128			//ÆÁÄ»ÏñËØ X_MAX
#define OLED_PIXEL_Y 64				//ÆÁÄ»ÏñËØ Y_MAX


//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_displayOFF(void);
void OLED_displayON(void);
void OLED_Init(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Draw_Point_3_3(uint8_t x, uint8_t y, uint8_t b);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_RamClear(void);
void OLED_ShowFloat(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode);
void OLED_ShowInt(u8 x, u8 y, int num, u8 size, u8 mode);

void OLED_Show_Char12x12(uint16_t left,uint16_t top,u8 *Char,uint16_t b);
void OLED_Show_Char16x16(uint16_t left,uint16_t top,u8 *Char,uint16_t b);
void OLED_Show_CustomChar(uint16_t left, uint16_t top, u8 *Char, uint16_t a, uint16_t b);

void WriteCmd(void);

static void OLED_IIC_Start(void);
static void OLED_IIC_Stop(void);
static void OLED_IIC_Wait_Ack(void);
static void OLED_WR_Byte(unsigned dat,unsigned cmd);

static void Write_IIC_Command(unsigned char IIC_Command);
static void Write_IIC_Data(unsigned char IIC_Data);
static void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Refresh_Gram(void);

void OLED_ShowFrame(void);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2, uint8_t b);
void OLED_Draw_Round(uint8_t x, uint8_t y, uint8_t r, uint8_t b);
void OLED_PartClear(u8 x0,u8 y0,u8 x1,u8 y1);
static void OLED_SetPos(unsigned char x, unsigned char y);

static unsigned char *reverse(unsigned char *s);
static unsigned char *my_itoa(long n);
static unsigned char *my_strcat(u8 * str1, u8 * str2);


#endif  
	 



