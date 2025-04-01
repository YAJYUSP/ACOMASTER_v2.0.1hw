#include "main.h"
#include "timer.h"
#include "tim.h"

#include "pwr_mngr.h"
#include "cw2015.h" 
#include "cs43131.h" 
#include "led.h"
#include "oled.h"
#include "gui.h"
#include "key.h"
#include "pwr_mngr.h"
#include "audio.h" 
#include "qcc5125.h" 
#include "tasks.h" 
#include "adau1761.h"


//使用定时器22实现微秒延时
void bsp_tim22_delay_us(uint16_t nus)
{
		uint16_t differ = 0xffff - nus - 5;
		HAL_TIM_Base_Start(&htim22);
		__HAL_TIM_SetCounter(&htim22, differ);
	  uint16_t timeout = 0;
		while(differ < 0xffff - 5)
		{
				timeout++;
				differ = __HAL_TIM_GetCounter(&htim22);
				if(timeout == 0xFFFF)
						break;
		}	
		HAL_TIM_Base_Stop(&htim22);
} 


//eq_chnl_e testing_chnl = eq_16000;

//PIT
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		static uint8_t time_cnt_1sec = 0;
		//5ms period 
		if (htim == (&htim21))                           
		{
				if(device_current_state == MAIN_UI)
				{
						if(IF_KEY_LEFT_PRSD)
								qcc5125_key_press(KEY_PREV, 1);
						else
								qcc5125_key_press(KEY_PREV, 0);
						
						if(IF_KEY_RIGHT_PRSD)
								qcc5125_key_press(KEY_NEXT, 1);
						else
								qcc5125_key_press(KEY_NEXT, 0);
				}
				else if(device_current_state == EQ_UI)
				{
						key_longpress_scan_5ms();
				}
		}
		

		//100ms period
		if (htim == (&htim6))                           
		{
			
				qcc5125_io_update_100ms();
			
//				//充电时显示的电池动画效果
//				if(sys_chg_info.chrg_state)
//				{
//					if(sys_chg_info.batt_soc_animation_state <100)
//						sys_chg_info.batt_soc_animation_state++;
//					else
//						sys_chg_info.batt_soc_animation_state = 0;
//				}
//				else
//						sys_chg_info.batt_soc_animation_state = 0;
//						
				
				//mute软延时功能，其mute持续时间400ms
				if(qcc5125_status.mute_delay_enable)
				{
						if(qcc5125_status.mute_delay_cnt < 1)
								qcc5125_status.mute_delay_cnt++;
						else
						{
								//延时时间到，解除mute,重新初始化音频链路
								qcc5125_status.mute_delay_cnt = 0;
								qcc5125_status.mute_delay_enable = 0;
								audio_enable();
						}
				}
				else
						qcc5125_status.mute_delay_cnt = 0;

				
				
				//EQ调整模式中，长按按键自动更改参数
				if(device_current_state == EQ_UI)
				{
						if(key_longpress_up.key_long_prsd_flag)
								if(eq_setting_boost[select_eq] < EQ_BOOST_MAX)
								{
										eq_setting_boost[select_eq] += 1;
										GUI_OLEDRFS_REQ = 1;
								}
						if(key_longpress_down.key_long_prsd_flag)
								if(eq_setting_boost[select_eq] > -EQ_BOOST_MAX)
								{
										eq_setting_boost[select_eq] -= 1;
										GUI_OLEDRFS_REQ = 1;
								}
				}
				
			
				//当系统初始化完成，才执行这里面的代码
				if(device_current_state != SYSTEM_INIT && device_current_state != INIT_UI)
				{
						if(time_cnt_1sec < 100)
								time_cnt_1sec++;
						else
						{
								//这里的代码每10000ms(10s)执行一次
								time_cnt_1sec = 0;
								//电量评估
								sys_chg_updt_info(&sys_chg_info);
						}		
						
						//刷新屏幕
						if(GUI_OLEDRFS_REQ)
						{
								OLED_Refresh_Gram();
								GUI_OLEDRFS_REQ = 0;
						}
				}
				
					
					
				//按键超时与息屏判断
				if(key_timeout_status.key_prsd_flag)
				{
						//若100ms内按键有被触发过，则清零超时时间
						key_timeout_status.key_prsd_flag = 0;
						key_timeout_status.key_timeout_not_prsd = 0;
				}
				else
				{
						//若100ms内按键没有被触发过
						//超时时间+1
						key_timeout_status.key_timeout_not_prsd ++;
						//超过超时时间，进入息屏模式
						if(key_timeout_status.key_timeout_not_prsd >= KEY_SLEEP_TIMEOUT_TIME)
						{
								key_timeout_status.key_timeout_not_prsd = 0;
								if(sys_ctrl.sys_status == SCREEN_ON)
										sys_ctrl.sys_status = SCREEN_OFF;
						}
				}
	
				
				
				//中键长按判断与关机，系统初始化时不进行关机判断
				if(IF_KEY_MID_PRSD && device_current_state != SYSTEM_INIT)
				{
						//长按持续1.5s，则关机
						if(key_middle_prsd_time < 15)
								key_middle_prsd_time ++;
						else
						{
								//关机
								GUI_power_off();
								sys_ctrl.sys_status = POWER_OFF;
						} 
				}
				else
						key_middle_prsd_time = 0;
				
				
				
				//根据sys_ctrl中的信息，进行系统电源状态实时切换（100ms）
				sys_ctrl_status_switch(&sys_ctrl);	
				
				
		}	
}
