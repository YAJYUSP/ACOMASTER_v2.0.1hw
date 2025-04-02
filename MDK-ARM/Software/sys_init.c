#include "main.h"
#include "sys_init.h"
#include "pwr_mngr.h"
#include "cw2015.h" 
#include "adau1761.h"
#include "cs43131.h" 
#include "led.h"
#include "oled.h"
#include "oledfont.h"
#include "gui.h"
#include "key.h"
#include "qcc5125.h" 
#include "eeprom.h"


void system_init(void)
{
		//系统上电/POR
		sys_pwrctrl_power_up();
		HAL_Delay(1);
	
		//长按开机延时
		uint8_t pwr_on_lpdelay = 0;
		if(IF_KEY_MID_PRSD)	//复位后如果OK键被按下，说明是从关机状态唤醒
		{
				//清理唤醒标志位
				__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	
			
				HAL_Delay(100);
				while(IF_KEY_MID_PRSD)//如果这时候还是按下去的状态，则记录按下的时间
				{
						pwr_on_lpdelay ++;
						HAL_Delay(100);
						if(pwr_on_lpdelay >= 10)
								break;
				}
				if(pwr_on_lpdelay < 10)
				{
						//继续关机休眠
						sys_ctrl.sys_status = POWER_OFF;
						sys_ctrl_status_switch(&sys_ctrl);
				}
		}
	
	
		HAL_Delay(100);
//		//开启电量计,解除休眠，初始化电量信息结构体
//		bsp_cw2015_slp(1);
//		//初始化cw2015电量计
//		bsp_cw2015_init();
//		HAL_Delay(5);
		sys_chg_info_init(&sys_chg_info);
		
		//初始化dsp的eq结构体，每次复位后调用一次即可
		bsp_adau1761_eq_init();
	
		//从eeprom读eq数据
		bsp_read_eqs_from_eeprom(eq_setting_boost);
		memcpy(eq_setting_boost_last, eq_setting_boost, 10);
		
		//打开QCC5125模块
		qcc5125_btn_press(KEY_PWR, 31);
		
		//初始化显示屏
		OLED_Init();
		OLED_RamClear();
		OLED_Refresh_Gram();
}


