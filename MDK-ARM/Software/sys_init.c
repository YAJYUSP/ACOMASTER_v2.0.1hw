#include "main.h"
#include "tim.h"

#include "sys_init.h"

#include "key.h"
#include "pwr_mngr.h"
#include "cw2015.h" 
#include "qcc5125.h" 
#include "oled.h"

#include "WouoUI_user.h"
#include "audio.h"


extern eq_param_t eq_channel[10];  // 10段EQ结构体的数组

void system_init(void)
{
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
	
		// 系统外设上电与POR
		sys_pwrctrl_power_up();
		HAL_Delay(100);
		
		// 打开100ms 周期定时器中断
		HAL_TIM_Base_Start_IT(&htim6);
		
		//打开QCC5125模块
		qcc5125_btn_press(KEY_PWR, 3200);

		//开启电量计,解除休眠，初始化电量信息结构体
		bsp_cw2015_slp(1);
		//初始化cw2015电量计
		bsp_cw2015_init();
		HAL_Delay(5);
		
//		sys_chg_info_init(&sys_chg_info);
		
		//初始化dsp的eq结构体，每次复位后调用一次即可
		audio_param_init(eq_channel);
	
		//从eeprom读eq数据
		audio_read_all_eqchnl_from_eeprom();
		
		//初始化显示屏
		OLED_Init();
		OLED_RamClear();
		OLED_Refresh_Gram();
		
		bsp_btn_scan_init();
		TestUI_Init();

		// 打开5ms 周期定时器中断
		HAL_TIM_Base_Start_IT(&htim21);
		
}


