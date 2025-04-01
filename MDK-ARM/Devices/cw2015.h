#ifndef  __CW2015_H
#define __CW2015_H

#include "stdint.h"

#define CW_ReadAddr    0xC5  //读命令
#define CW_WriteAddr   0XC4	//写命令		

#define VERSION             0x00  // 读  芯片ID

#define VCELL_H 	 0x02	// 读  14位ADC电池电压高位
#define VCELL_L 	 0x03	// 读  14位ADC电池电压低位
#define VCELL_STEP 	 0.305F	// VCELL读数步进值

#define SOC_B      0x04  // 读  提供以%为单位的电量数据
#define SOC          0x05  // 读  更精确的电量显示1/256% 

#define RRT_H      0x06   // 读  使用剩余时间高位
#define RRT_L       0x07  // 读  使用剩余时间低位
                              //[15]ALRT SOC寄存器阈值警报标志位（只能由IC清除）
                              //[12-0]剩余时间 根据当前数据计算剩余时间 单位分钟
#define CONFIG    0x08  // 读写      默认：0x50 10%   0xA0 20%
                              //[7-3]SOC警报阈值设置 在ALRT产生中断  
															//  [1]UPG 用于指示电池信息更新状态标志位
#define MODE      0x0A  // 读写      默认：0x00
                              // [76]睡眠模式       2位控制  11强制进入睡眠模式  默认：00醒来
															// [54]QSTAT快速启动  2位控制  11开始                默认：00
															//[3-0]电源复位       4位控制  1111复位                  默认：0000


void bsp_cw2015_init(void);
float bsp_cw2015_read_volt(void);
uint8_t bsp_cw2015_read_soc(uint8_t mode);
uint16_t bsp_cw2015_get_time(void);
uint8_t bsp_cw2015_get_alrt(void);
uint8_t bsp_cw2015_read_id(void);
void bsp_cw2015_set_alrt(uint8_t val);
void bsp_cw2015_slp(uint8_t ctrl);

extern uint8_t cw2015_iic_musk;


static uint8_t CW2015_ReadOneByte(uint8_t ReadAddr);
static uint8_t CW2015_WriteOneByte(uint8_t WriteAddr,uint8_t Data);







#endif
















