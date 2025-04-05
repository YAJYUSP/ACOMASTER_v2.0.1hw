#include "main.h"
#include "tasks.h"

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
				
						*DeviceState = MAIN_UI;
						*Device_LastState = SYSTEM_INIT;
						break;

				case MAIN_UI:
		
						*Device_LastState = MAIN_UI;
						break;
				
				case EQ_UI_TRANSITION:

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

