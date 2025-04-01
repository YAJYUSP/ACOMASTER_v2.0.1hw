#include "cw2015.h" 
#include "simi2c.h"
#include "main.h"


//电量计的iic总线屏蔽位，置1时不进行电量读取
uint8_t cw2015_iic_musk = 0;

void bsp_cw2015_init(void)
{
		bsp_cw2015_slp(1);
		bsp_cw2015_set_alrt(20);//设置电量报警阈值
}

//芯片睡眠模式,0进入，1退出
void bsp_cw2015_slp(uint8_t ctrl)
{
		if(!ctrl)
				CW2015_WriteOneByte(MODE,0x03<<6);
		else
				CW2015_WriteOneByte(MODE,0x03<<4);//qstat
}

uint8_t bsp_cw2015_read_id(void)      //读CW2015的ID
{
		uint8_t temp=0;
		temp=CW2015_ReadOneByte(0x00);
		return temp;
}


//读电池电压,16位寄存器最高16383，步进305uV，返回值mV
float bsp_cw2015_read_volt(void) 
{
		uint16_t temp_H=0;
		uint16_t temp_L=0;
		temp_H=CW2015_ReadOneByte(VCELL_H);//读高8位
		temp_L=CW2015_ReadOneByte(VCELL_L);//读低8位
		
		temp_H = (temp_H<<8)+temp_L;
		return temp_H * VCELL_STEP;
}


//读电池电量
//mode=0  百分比  mode=1  1/256
uint8_t bsp_cw2015_read_soc(uint8_t mode)
{
		uint8_t temp=0;
		if(mode==0)
				temp=CW2015_ReadOneByte(SOC_B);//读百分比的电量
		else if(mode==1)
				temp=CW2015_ReadOneByte(SOC);//读1/256的电量
		return temp;
}


//读取剩余使用时间 分钟
uint16_t bsp_cw2015_get_time(void)
{
	uint16_t temp_H=0;
  uint16_t temp_L=0;
  temp_H=CW2015_ReadOneByte(RRT_H);//读高8位
	temp_H=temp_H - 0xE000;
	temp_L=CW2015_ReadOneByte(RRT_L);//读低8位
	 
	return (temp_H<<8)+temp_L;
}


//SOC寄存器阈值警报标志位 
uint8_t bsp_cw2015_get_alrt(void)
{ 
		uint8_t temp=0;
		temp=CW2015_ReadOneByte(RRT_H);//读高8位
		temp=temp>>7;
		return temp;
}

//设置SOC寄存器警报阈值,最大输入31（即31%）
void bsp_cw2015_set_alrt(uint8_t val)
{ 
		if(val > 31)
				val = 31;
		CW2015_WriteOneByte(CONFIG,val<<3);
}


//返回值  :读到的数据
static uint8_t CW2015_ReadOneByte(uint8_t ReadAddr)
{				  
	  uint8_t temp=0;		  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(CW_WriteAddr);//发送写命令
		IIC_Wait_Ack();
	
		IIC_Send_Byte(ReadAddr);//发送寄存器地址
		IIC_Wait_Ack();	 

	  IIC_Start(); 
		IIC_Send_Byte(CW_ReadAddr);           //发送读命令	
	  IIC_Wait_Ack();	
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	 
		return temp;
}


static uint8_t CW2015_WriteOneByte(uint8_t WriteAddr,uint8_t Data)
{
    IIC_Start();
		IIC_Send_Byte(CW_WriteAddr );	    //发送写命令
		if(IIC_Wait_Ack())	//等待应答
	  {
				IIC_Stop();		 
				return 1;		
	  }
	  IIC_Send_Byte(WriteAddr);//发送寄存器地址
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(Data);     //发送字节							   
	  if(IIC_Wait_Ack())	//等待应答
	  {
				IIC_Stop();		 
				return 1;		
	  }  		    	   
    IIC_Stop();//产生一个停止条件 
		return 0;	 
}






