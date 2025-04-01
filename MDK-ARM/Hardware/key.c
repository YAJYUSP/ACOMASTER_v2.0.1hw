#include "main.h"
#include "key.h"
#include "gui.h"
#include "oled.h"
#include "pwr_mngr.h"
#include "cw2015.h" 
#include "qcc5125.h" 
#include "audio.h"
#include "tasks.h"
#include "adau1761.h"
#include "rtc.h"


key_timeout_status_t key_timeout_status;
key_status_t key_status;
key_longpress_t key_longpress_up, key_longpress_down;

//中键被按下的持续时间，用于长按判断
uint16_t key_middle_prsd_time = 0;

//按键扫描
void bsp_key_scan(key_status_t *key)
{
		key ->if_key_up_prsd = IF_KEY_UP_PRSD;
		key ->if_key_down_prsd = IF_KEY_DOWN_PRSD;
		key ->if_key_left_prsd = IF_KEY_LEFT_PRSD;
		key ->if_key_right_prsd = IF_KEY_RIGHT_PRSD;
		key ->if_key_mid_prsd = IF_KEY_MID_PRSD;
}
//记录上一次按键值
void bsp_key_record(key_status_t *key)
{
		key ->if_key_up_prsd_last = key ->if_key_up_prsd;
		key ->if_key_down_prsd_last = key ->if_key_down_prsd;
		key ->if_key_left_prsd_last = key ->if_key_left_prsd;
		key ->if_key_right_prsd_last = key ->if_key_right_prsd;
		key ->if_key_mid_prsd_last = key ->if_key_mid_prsd;
}
//按键长按判定
void key_longpress_scan_5ms(void)
{
		if(IF_KEY_UP_PRSD)
		{
				if(key_longpress_up.key_longpress_time < 150)
						key_longpress_up.key_longpress_time++;
				else
						key_longpress_up.key_long_prsd_flag = 1;
		}
		else
		{
				key_longpress_up.key_long_prsd_flag = 0;
				key_longpress_up.key_longpress_time = 0;
		}
		
		if(IF_KEY_DOWN_PRSD)
		{
				if(key_longpress_down.key_longpress_time < 150)
						key_longpress_down.key_longpress_time++;
				else
						key_longpress_down.key_long_prsd_flag = 1;
		}
		else
		{
				key_longpress_down.key_long_prsd_flag = 0;
				key_longpress_down.key_longpress_time = 0;
		}
}


extern uint16_t auto_pwroff_timeout;
//按键中断回调
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		key_timeout_status.key_prsd_flag = 1;
		//如果系统休眠，则恢复，忽略按键操作
		if(sys_ctrl.sys_status == SCREEN_OFF)
		{
				//重新配置系统时钟
				HAL_Init();
				SystemClock_Config();
				//关闭RTC唤醒中断
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
				//清零自动关机计数器
				auto_pwroff_timeout = 0;
				sys_ctrl.sys_status = SCREEN_ON;
				HAL_Delay(100);
				//开启屏幕显示
				OLED_displayON();
				HAL_Delay(300);
				return;
		}
		else
		{
				if(IF_KEY_UP_PRSD)
				{
						if(device_current_state == EQ_UI)
						{
								HAL_Delay(20);
								if(IF_KEY_UP_PRSD)
								{
										if(eq_setting_boost[select_eq] < EQ_BOOST_MAX)
												eq_setting_boost[select_eq] += 1;
										GUI_OLEDRFS_REQ = 1; //屏幕刷新请求
								}
						}
				}
				
				else if(IF_KEY_DOWN_PRSD)
				{
						if(device_current_state == EQ_UI)
						{
								HAL_Delay(20);
								if(IF_KEY_DOWN_PRSD)
								{
										if(eq_setting_boost[select_eq] > -EQ_BOOST_MAX)
												eq_setting_boost[select_eq] -= 1;
										GUI_OLEDRFS_REQ = 1; //屏幕刷新请求
								}
						}
				}
				
				else if(IF_KEY_LEFT_PRSD)
				{
						if(device_current_state == EQ_UI)
						{
								HAL_Delay(20);
								if(IF_KEY_LEFT_PRSD)
								{
										if(select_eq > eq_63)
												select_eq -=1;
										else 
												select_eq = eq_16000;
										GUI_OLEDRFS_REQ = 1; //屏幕刷新请求
								}
						}
				}
				
				else if(IF_KEY_RIGHT_PRSD)
				{
						if(device_current_state == EQ_UI)
						{
								HAL_Delay(20);
								if(IF_KEY_RIGHT_PRSD)
								{
										if(select_eq < eq_16000)
												select_eq += 1;
										else 
												select_eq = eq_63;
										GUI_OLEDRFS_REQ = 1; //屏幕刷新请求
								}
						}
				}
				 
				else if(IF_KEY_MID_PRSD)
				{
						//当进入主页面
						if(device_current_state == MAIN_UI)
						{
								HAL_Delay(50);
								if(IF_KEY_MID_PRSD)
										qcc5125_btn_press(KEY_PLAY, 2);
						}	
				}		
		}
}




