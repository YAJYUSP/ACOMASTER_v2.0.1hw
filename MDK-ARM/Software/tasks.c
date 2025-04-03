#include "main.h"
#include "tasks.h"
#include "WouoUI_user.h"

#include "sys_init.h"
#include "gui.h"
#include "key.h"
#include "tim.h"
#include "oled.h"
#include "oledfont.h"  
#include "qcc5125.h" 
#include "cw2015.h" 
#include "cs43131.h" 
#include "audio.h"
#include "adau1761.h"

#include "eeprom.h"


//设备当前/上一次的状态值
device_state_e device_current_state, device_last_state = SYSTEM_INIT;

//设备程序主状态机
void main_FSM(device_state_e *DeviceState, device_state_e *Device_LastState, uint8_t TaskPeriod)
{
		switch(*DeviceState)
		{
			
				case SYSTEM_INIT:
						//“上电/开机后的系统初始化”
						system_init();
						*DeviceState = INIT_UI;
						*Device_LastState = SYSTEM_INIT;
						break;
				
				case INIT_UI:
//						//“显示启动UI”
//						GUI_powerUp();
//						OLED_RamClear();
//						OLED_Refresh_Gram();
				
						bsp_btn_scan_init();
				
//						GUI_main_page();
						//5ms PIT
						HAL_TIM_Base_Start_IT(&htim21);
						//100ms PIT
						HAL_TIM_Base_Start_IT(&htim6);
				
				
						TestUI_Init();
				
						*DeviceState = MAIN_UI;
						*Device_LastState = INIT_UI;
						break;
						
				case MAIN_UI:
//						//“播放页面UI”

//						//更新模块输出引脚状态
//						qcc5125_status.is_connected = IF_BL_CNED;
//						qcc5125_status.is_music_playing = IF_MUSIC_PLAYING;
//				
//						//当模块开始播放，说明MCLK时钟由失效变为有效，此时给DSP以及CODEC进行配置
//						if(qcc5125_status.is_music_playing == 1 && qcc5125_status.is_music_playing_last == 0)
//						{
//								//若没初始化过，则进行初始化并修改标志位，每次复位后最多执行一次
//								if(!audio_init_flag)
//								{
//										qcc5125_status.mute_delay_enable = 1;
//										audio_init_flag = 1;
//								}
//						}
//				
//						//更新状态栏显示信息
//						GUI_update_status_bar(&sys_chg_info, 0);
//						//更新主菜单按钮显示
//						GUI_update_button();
//				
//						//上翻进入EQ设置页面
//						if(IF_KEY_UP_PRSD)
//						{
//								OLED_RamClear();
//								*DeviceState = EQ_UI_TRANSITION;
//						}
//				
//						//记录上一次的模块状态
//						qcc5125_status.is_connected_last = qcc5125_status.is_connected;
//						qcc5125_status.is_music_playing_last = qcc5125_status.is_music_playing;
				
						*Device_LastState = MAIN_UI;
						break;
				
				case EQ_UI_TRANSITION:
//						//“EQ设置过渡动画”
//						OLED_ShowString(40,18, (uint8_t *)"EQ",16, 1);
//						for(uint8_t i=0; i<2; i++)
//								OLED_Show_CustomChar(58+16*i, 32,(uint8_t *)chn_she_16_16[i],16,1);
//						for(uint8_t i=0; i<3; i++)
//						{
//								OLED_Show_Char12x12(38+20*i,10,(uint8_t *)icon_filled_ball_12_12,1);
//								HAL_Delay(100);
//								OLED_Refresh_Gram();
//						}
//						
//						HAL_Delay(300);
//						OLED_RamClear();
//						GUI_eq(eq_setting_boost);
//						GUI_oled_draw_wave(eq_setting_boost, eq_setting_boost_last);	
//						gui_update_eq_freq(select_eq, select_eq_last);
//						OLED_Refresh_Gram();
						
						
						*DeviceState = EQ_UI;
						*Device_LastState = EQ_UI_TRANSITION;
						break;
						
				case EQ_UI:
//						//“EQ设置页面”
//						GUI_oled_draw_wave(eq_setting_boost, eq_setting_boost_last);
//						
//						gui_update_eq_freq(select_eq, select_eq_last);
//				
//						select_eq_last = select_eq;
//						bsp_adau1761_eq_save_last(eq_setting_boost, eq_setting_boost_last);
//						
//						//刷新屏幕
//						if(GUI_OLEDRFS_REQ)
//						{
//								OLED_Refresh_Gram();
//								GUI_OLEDRFS_REQ = 0;
//						}
//						
//						//按下中键，回到主页面
//						if(IF_KEY_MID_PRSD)
//						{
//								//将eq信息写入eeprom
//								bsp_write_eqs_to_eeprom(eq_setting_boost);
//								*DeviceState = MAIN_UI;
//								GUI_main_page();
//						}
						*Device_LastState = EQ_UI;
						break;
				
				
				case EQ_SAVING_UI:

						break;
						
				default:
						break;
				
		}
}

