#include <main.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "oled.h"
#include "oledfont.h"  	

#include <i2c.h>
#include <dma.h>
#include "timer.h"  	

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
uint8_t OLED_GRAM[128][8];

void OLED_Init(void)
{ 	
  OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD);// contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

//更新显存到OLED
void OLED_Refresh_Gram(void)
{
		u8 i,n;
		for(i=0;i<8;i++)
		{
				OLED_WR_Byte (0xb0+i,OLED_CMD); //设置页地址（0~7）
				OLED_WR_Byte (0x00,OLED_CMD); //设置显示位置—列低地址
				OLED_WR_Byte (0x10,OLED_CMD); //设置显示位置—列高地址
				HAL_Delay(1);

				uint8_t txData[129];
				txData[0] = 0x40;	//表明要发送的帧为数据帧
				for(n=0;n<128;n++) txData[n+1] = OLED_GRAM[n][i];
				while(HAL_DMA_GetState(hi2c1.hdmatx) != HAL_DMA_STATE_READY);
				HAL_I2C_Master_Transmit_DMA(&hi2c1, OLED_SLAVE_ADDR, txData, 129);

				HAL_Delay(1);
		}
}

//将一帧显存数组更新到OLED,要求显存结构为uint8_t buff[8][128]
//这里专门为wouoUI定制此函数
void OLED_Refresh_FrameBuffer(uint8_t buff[][128])
{
		u8 i,n;
		for(i=0;i<8;i++)
		{
				OLED_WR_Byte (0xb0+i,OLED_CMD); //设置页地址（0~7）
				OLED_WR_Byte (0x00,OLED_CMD); //设置显示位置—列低地址
				OLED_WR_Byte (0x10,OLED_CMD); //设置显示位置—列高地址
				HAL_Delay(1);

				uint8_t txData[129];
				txData[0] = 0x40;	//表明要发送的帧为数据帧
				for(n=0;n<128;n++) txData[n+1] = buff[i][n];
				while(HAL_DMA_GetState(hi2c1.hdmatx) != HAL_DMA_STATE_READY);
				HAL_I2C_Master_Transmit_DMA(&hi2c1, OLED_SLAVE_ADDR, txData, 129);

				HAL_Delay(1);
		}
}

 //关闭屏幕显示
void OLED_displayOFF(void)
{
		OLED_WR_Byte(0xAE,OLED_CMD);//--display off
}

 //关闭屏幕显示
void OLED_displayON(void)
{
		OLED_WR_Byte(0xAF,OLED_CMD);//--display on
}


//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	   
}


//画线函数
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2, uint8_t b)
{
	uint8_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		OLED_DrawPoint(uRow,uCol,b);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    


void OLED_Draw_Round(uint8_t x, uint8_t y, uint8_t r, uint8_t b)
{
		uint8_t vx,vy;
		uint16_t ang;
		y=63-y;
		for(ang=0; ang<360; ang++)
		{
				vx = x+r*cos(ang*PI/180);
				vy = y+r*sin(ang*PI/180);
				OLED_DrawPoint(vx,vy,b);
		}
		OLED_Refresh_Gram();
}

void OLED_Draw_Point_3_3(uint8_t x, uint8_t y, uint8_t b)
{
		OLED_DrawPoint(x-1, y+1, b);
		OLED_DrawPoint(x-1, y, b);
		OLED_DrawPoint(x-1, y-1, b);
		OLED_DrawPoint(x, y+1, b);
		OLED_DrawPoint(x, y, b);
		OLED_DrawPoint(x, y-1, b);
		OLED_DrawPoint(x+1, y+1, b);
		OLED_DrawPoint(x+1, y, b);
		OLED_DrawPoint(x+1, y-1, b);
}




 
 void OLED_ShowFrame(void)
 {
	 OLED_DrawLine(0,0,127,0,1);
	 OLED_DrawLine(127,0,127,63,1);
	 OLED_DrawLine(127,63,0,63,1);
	 OLED_DrawLine(0,63,0,0,1); 
 }
 /**************************************************************************************************/


 //--------------------------------------------------------------
// Prototype      : void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~63); int num 显示的int整型; Size:字号(12/16/24)  
// mode:0(OLED_DISPLAYCHAR_REVERSE)反白显示;1(OLED_DISPLAYCHAR_NORMAL),正常显示  
// Description    : 
//--------------------------------------------------------------
void OLED_ShowInt(u8 x, u8 y, int num, u8 size, u8 mode)
{
	unsigned char *ch = my_itoa(num);
	OLED_ShowString(x, y, ch, size, mode);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~63); float num 显示的float型; 
// pointNum		  : 小数点后保留位数(0~5)
// Size:字号(12/16/24)  
// mode:0(OLED_DISPLAYCHAR_REVERSE)反白显示;1(OLED_DISPLAYCHAR_NORMAL),正常显示  
// Description    : 
//--------------------------------------------------------------
void OLED_ShowFloat(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode)
{
	unsigned char ch1[50],ch2[50];
	unsigned char *ptemp;
	unsigned i=0,j=0;
	long t1,t2;
	float ftemp;
	t1 = num/1;
	ftemp = num - t1;
	for(i = 0; i < pointNum;i++)
	{
		ftemp *= 10;
	}
	t2 = (long)ftemp;
	
	ptemp = my_itoa(t1);
	for(i = 0; i < 50;i++)	ch1[i] = *ptemp++;
	ptemp = my_itoa(t2);
	for(i = 0; i < 50;i++)	ch2[i] = *ptemp++;
	
	while(ch1[j] != '\0')
	{
		j++;
	}
	ch1[j] = '.';
	ptemp = my_strcat(ch1, ch2);
	OLED_ShowString(x, y, ptemp, size, mode);
}



//显示12x12的自定义字符
void OLED_Show_Char12x12(uint16_t left,uint16_t top,u8 *Char,uint16_t b)
{
		u8 i = 0, j = 0, x = 0, b0 = 0;
		uint16_t ptr = 0;	
		//单个显示
		for(i = 12; i > 0; i--)//页
		{
				for(j = 0; j < 12; j++)//列
				{
						b0 = ((Char[ptr]<<x)&0x80);
						if(ptr%2)
								if(x<3)
										x ++;
								else
								{
										ptr ++;
										x = 0;
								}
						else
								if(x<7)
										x ++;
								else
								{
										ptr ++;
										x = 0;
								}
								if(b0)
										OLED_DrawPoint(left+j, top+i, b); 
				}
		}
}


//显示axa的自定义字符,a为8的整数倍
void OLED_Show_CustomChar(uint16_t left, uint16_t top, u8 *Char, uint16_t a, uint16_t b)
{
		u8 i = 0, j = 0, x = 0, b0 = 0;
		uint16_t ptr = 0;	
		if(a <8 || a%8 != 0)
				return;
		//单个显示
		for(i = a; i > 0; i--)//页
		{
				for(j = 0; j < a; j++)//列
				{
						b0 = ((Char[ptr]<<x)&0x80);
						if(b0)
								OLED_DrawPoint(left+j, top+i, b);
						x++;
						if(x == 8)
						{
								ptr++;
								x=0;
						}
				}
		}
}




 /*********************************************
 Function	:void OLED_RamClear(void)
 Description:将GRAM全置为0  清空显存
 Input	: void
 Return	: void
 Author	: firestaradmin
 **********************************************/
void OLED_RamClear(void)
{
		for(uint8_t i=0; i<128; i++)
				memset(OLED_GRAM[i],0,8);
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//t:1(OLED_LED_LIGHTUP) 填充 ; 0(OLED_LED_EXTINGUISH),清空	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
		u8 x,y;  
		for(x=x1;x<=x2;x++)
		{
				for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
		}													    
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0(OLED_DISPLAYCHAR_REVERSE)反白显示;1(OLED_DISPLAYCHAR_NORMAL),正常显示  				 
//size:选择字体 8/12/16/24 (列高)
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值	
	if(size == 8)csize = 5;	
    for(t=0;t<csize;t++)
    {   
		if(size==16)temp=ASC8X16[chr][t];	//调用1608字体
		else if(size==8)temp=ASC5X8[chr][t];
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,64-y,mode);
			else OLED_DrawPoint(x,64-y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//mode:0(OLED_DISPLAYCHAR_REVERSE)反白显示;1(OLED_DISPLAYCHAR_NORMAL),正常显示  				 
//size:选择字体 12/16/24
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
	//u8 csize=(size/8+((size%8)?1:0))*(size/2);
	if(size != 8)	
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=size/2;
			p++;
		} 
	}	
	else 
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=5;
			p++;
		} 
	}	
}


/**********************************************
// IIC Write Command
**********************************************/
static void Write_IIC_Command(unsigned char IIC_Command)
{

	uint8_t cmd = IIC_Command;
	HAL_I2C_Mem_Write(&hi2c1, OLED_SLAVE_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0xffff);
}
/**********************************************
// IIC Write Data
**********************************************/
static void Write_IIC_Data(unsigned char IIC_Data)
{

	uint8_t data = IIC_Data;
	HAL_I2C_Mem_Write(&hi2c1, OLED_SLAVE_ADDR, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xffff);
}



static void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
		if(cmd)
		 {
				Write_IIC_Data(dat);
		 }
		else 
		 {
				Write_IIC_Command(dat);
		 }
}
	

/*********************************************
Function:	unsigned char *reverse(unsigned char *s)
Description:将字符串顺序颠倒
Input:		unsigned char * ：要颠倒的字符串
Return:		unsigned char* :转换后的字符串指针
Author:		firestaradmin
**********************************************/
static unsigned char *reverse(unsigned char *s)
{
    unsigned char temp;
    unsigned char *p = s;    //p指向s的头部
    unsigned char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;
    
    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}


/*********************************************
Function:	unsigned char *my_itoa(int n)
Description:将int型转换为unsigned char*字符串
Input:		int n ：要转换的数
Return:		unsigned char* :转换后的字符串指针
Calls:		unsigned char *reverse(unsigned char *s)
Author:		firestaradmin
**********************************************/
static unsigned char *my_itoa(long n)
{
    int i = 0,isNegative = 0;
    static unsigned char s[50];      //必须为static变量，或者是全局变量
    if((isNegative = n) < 0) //如果是负数，先转为正数
    {
        n = -n;
    }
    do      //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //如果是负数，补上负号
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //最后加上字符串结束符
    return reverse(s);
}
/*********************************************
Function:	unsigned char *my_strcat(u8 * str1, u8 * str2)
Description:将str2拼接到str1末尾
Input:		str1 str2
Return:		unsigned char* :转换后的字符串指针
Calls:		
Author:		firestaradmin
**********************************************/
static unsigned char *my_strcat(u8 * str1, u8 * str2)
{
		u8* pt = str1;
		while(*str1 != '\0') str1++;
		while(*str2 != '\0') *str1++ = *str2++;
		*str1 = '\0';
		return pt;
}

