

#include "main.h"
#include <stdio.h>
#include "gui.h"
#include "oledfont.h"  	
#include "oled.h"

#include "key.h"
#include "pwr_mngr.h"
#include "timer.h"
#include "qcc5125.h" 
#include "cs43131.h" 
#include "cw2015.h" 
#include "audio.h"
#include "adau1761.h"


//当前选中的，需要更改的eq段
eq_chnl_e select_eq, select_eq_last = eq_63;

//GUI刷新显示屏请求，被置1时刷新一次显示屏
uint8_t GUI_OLEDRFS_REQ = 0;

//各段eq对应线段x坐标：
const uint8_t eq_gui_x_coor[10] = 
{
		10, 22, 34, 46, 58, 70, 82, 94, 106, 118
};




//显示等待连接蓝牙画面
void GUI_wait_until_connect(void)
{
		OLED_RamClear();
		for(uint8_t i=0; i<6; i++)
				//OLED_Show_Char12x12(12+12*i,40,(uint8_t *)chn_deng_12_12[i],1);
		OLED_ShowString(84,10,(uint8_t *)"....", 16,1);
		OLED_Refresh_Gram();
		HAL_Delay(500);
		//若未连接，则等待至连接成功
		while(!IF_BL_CNED)
		{
				for(uint8_t i=0; i<3; i++)
				{
						//OLED_Show_Char12x12(38+20*i,10,(uint8_t *)icon_filled_ball_12_12,1);
						HAL_Delay(100);
						OLED_Refresh_Gram();
				}
				for(uint8_t i=0; i<3; i++)
				{
						//OLED_Show_Char12x12(38+20*i,10,(uint8_t *)icon_filled_ball_12_12,0);
						HAL_Delay(100);
						OLED_Refresh_Gram();
				}
		}
		//连接成功
		OLED_RamClear();
		//显示连接成功字样
		for(uint8_t i=0; i<3; i++)
				//OLED_Show_Char12x12(48+12*i,40,(uint8_t *)chn_yi_12_12[i],1);
		
		OLED_Refresh_Gram();
		HAL_Delay(1000);
}

//重绘主页面
void GUI_main_page(void)
{
		OLED_RamClear();
		OLED_ShowString(0, 2, (const uint8_t*)"Hi-Res", 8, 1);
//		OLED_Show_CustomChar(60,0, (uint8_t *)icon_triangle_up_8_8, 8, 1);
//		OLED_Show_CustomChar(24,20,(uint8_t *)icon_prev_unchecked_16_16,16,1);
//		OLED_Show_CustomChar(60,20,(uint8_t *)icon_play_unchecked_16_16,16,1);
//		OLED_Show_CustomChar(88,20,(uint8_t *)icon_next_unchecked_16_16,16,1);
		
	  //强制重绘状态栏
		GUI_update_status_bar(&sys_chg_info, 1);
		GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
}



//更新状态栏的显示（音量与电量） force_rfs:强制重绘
void GUI_update_status_bar(sys_chg_t *chg_info, uint8_t force_rfs)
{
		//充电状态判断
		if(!HAL_GPIO_ReadPin(CHARGING_GPIO_Port, CHARGING_Pin))		
		{
				if(HAL_GPIO_ReadPin(CHRG_DONE_GPIO_Port, CHRG_DONE_Pin))		
						chg_info->chrg_state = 1;
		}
		else
				chg_info->chrg_state = 0;
	
		if(force_rfs)
		{
			/* 如果正在充电，则无需显示电池soc而是小闪电与电池logo*/
			if(sys_chg_info.chrg_state)
			{
//				OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1); //擦除soc
//				OLED_Show_CustomChar(LIGHTNING_DISP_X,46,(uint8_t *)icon_lightning_16_16,16,1);
//				OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
			}
			/* 如果没在充电，则显示电池soc与电池logo，擦除小闪电*/
			else
			{
//				uint8_t soc_disp_len = sys_chg_info.soc/10;
//				OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//				OLED_DrawLine(121, 60, 121-soc_disp_len, 60, 1);
//				OLED_DrawLine(121, 59, 121-soc_disp_len, 59, 1);
//				OLED_DrawLine(121, 58, 121-soc_disp_len, 58, 1);
//				OLED_DrawLine(121, 57, 121-soc_disp_len, 57, 1);
//				//电池soc
//				OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1);
//				OLED_ShowInt(SOC_DISP_X,3,sys_chg_info.soc, 8, 1);
				
//				OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1);
//				OLED_Show_CustomChar(LIGHTNING_DISP_X,46,(uint8_t *)icon_lightning_16_16,16,0);
			}

			if(qcc5125_status.is_connected)
				//OLED_Show_Char12x12(BLUETOOTH_DISP_X,50,(uint8_t *)icon_bluetooth_12_12,1);
			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
			
			return;
		}
		
		
		
		//显示充电状态小闪电
		if(sys_chg_info.chrg_state && !sys_chg_info.chrg_state_last)
		{
			/* 开始充电时 */
//			OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1); //擦除soc
//			OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//			OLED_Show_CustomChar(LIGHTNING_DISP_X,46,(uint8_t *)icon_lightning_16_16,16,1);
//			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
		}
		if(!sys_chg_info.chrg_state && sys_chg_info.chrg_state_last)
		{
			/* 充电结束时 */
			
//			uint8_t soc_disp_len = sys_chg_info.soc/10;
//			OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//			OLED_DrawLine(121, 60, 121-soc_disp_len, 60, 1);
//			OLED_DrawLine(121, 59, 121-soc_disp_len, 59, 1);
//			OLED_DrawLine(121, 58, 121-soc_disp_len, 58, 1);
//			OLED_DrawLine(121, 57, 121-soc_disp_len, 57, 1);
//			//电池soc
//			OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1);
//			OLED_ShowInt(SOC_DISP_X,3,sys_chg_info.soc, 8, 1);
//			OLED_Show_CustomChar(LIGHTNING_DISP_X,46,(uint8_t *)icon_lightning_16_16,16,0);
//			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
		}
		
		//电池图标与电池soc
		if(!sys_chg_info.chrg_state)
		{
			//不充电时，显示soc与电量logo
			if(sys_chg_info.soc != sys_chg_info.soc_last)
			{
//				OLED_Show_CustomChar(108,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//				uint8_t soc_disp_len = sys_chg_info.soc/10;
//				OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//				OLED_DrawLine(121, 60, 121-soc_disp_len, 60, 1);
//				OLED_DrawLine(121, 59, 121-soc_disp_len, 59, 1);
//				OLED_DrawLine(121, 58, 121-soc_disp_len, 58, 1);
//				OLED_DrawLine(121, 57, 121-soc_disp_len, 57, 1);
////				//电池soc
//				OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1);
//				OLED_ShowInt(SOC_DISP_X,3,sys_chg_info.soc, 8, 1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//					OLED_ShowString(SOC_DISP_X,3,(uint8_t *)"   ",8,1);
//					OLED_ShowInt(SOC_DISP_X,3,sys_chg_info.battery_volt, 8, 1);
//					GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
			}
		}
//		else if(sys_chg_info.batt_soc_animation_state != sys_chg_info.batt_soc_animation_state_last)
//		{
//			//充电时，只显示充电动画效果，停止显示soc与电量logo
//			uint8_t soc_disp_len = sys_chg_info.batt_soc_animation_state/10;
//			OLED_Show_CustomChar(BATT_DISP_X,46,(uint8_t *)icon_battery_empty_16_16,16,1);
//			OLED_DrawLine(121, 60, 121-soc_disp_len, 60, 1);
//			OLED_DrawLine(121, 59, 121-soc_disp_len, 59, 1);
//			OLED_DrawLine(121, 58, 121-soc_disp_len, 58, 1);
//			OLED_DrawLine(121, 57, 121-soc_disp_len, 57, 1);
			
//			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}

		sys_chg_info.soc_last = sys_chg_info.soc;
		sys_chg_info.chrg_state_last = sys_chg_info.chrg_state;
		sys_chg_info.batt_soc_animation_state_last = sys_chg_info.batt_soc_animation_state;
			
		//显示蓝牙连接图标
		if(qcc5125_status.is_connected && !qcc5125_status.is_connected_last)
		{
			//OLED_Show_Char12x12(BLUETOOTH_DISP_X,50,(uint8_t *)icon_bluetooth_12_12,1);
			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
		}
		else if(!qcc5125_status.is_connected && qcc5125_status.is_connected_last)
		{
			//OLED_Show_Char12x12(BLUETOOTH_DISP_X,50,(uint8_t *)icon_bluetooth_12_12,0);
			GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
		}

}

//显示关机页面
void GUI_power_off(void)
{
		OLED_RamClear();
		for(uint8_t i=0; i<3; i++)
//				OLED_Show_CustomChar(42+16*i,40,(uint8_t *)chn_guan_12_12[i],16,1);
		OLED_Refresh_Gram();
		for(uint8_t i=0; i<3; i++)
		{
				//OLED_Show_Char12x12(38+20*i,10,(uint8_t *)icon_filled_ball_12_12,1);
				HAL_Delay(100);
				OLED_Refresh_Gram();
		}
		for(uint8_t i=0; i<3; i++)
		{
				//OLED_Show_Char12x12(38+20*i,10,(uint8_t *)icon_filled_ball_12_12,0);
				HAL_Delay(100);
				OLED_Refresh_Gram();
		}
}



//更新主页面按钮的显示
void GUI_update_button(void)
{
//		//扫描按键
//		bsp_key_scan(&key_status);
		
//		//开始播放音乐时
//		if(qcc5125_status.is_music_playing == 1 && qcc5125_status.is_music_playing_last == 0)
//		{
//				// 显示暂停icon
//				OLED_Fill(57, 20, 76, 36, 0);
//				OLED_Show_CustomChar(57,20,(uint8_t *)icon_pause_unchecked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		//暂停音乐时
//		else if(qcc5125_status.is_music_playing == 0 && qcc5125_status.is_music_playing_last == 1)
//		{
//				// 显示播放icon
//				OLED_Fill(57, 20, 76, 36, 0);
//				OLED_Show_CustomChar(60,20,(uint8_t *)icon_play_unchecked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		if(IF_MUSIC_PLAYING)//音乐正在播放时，要显示的是暂停icon
//		{
//			if(key_status.if_key_mid_prsd && !key_status.if_key_mid_prsd_last)//按下中键
//			{
//					OLED_Fill(57, 20, 76, 36, 0);
//					OLED_Show_CustomChar(57,20,(uint8_t *)icon_pause_checked_16_16,16,1);
//					GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//			}
//			if(!key_status.if_key_mid_prsd && key_status.if_key_mid_prsd_last)//释放中键
//			{
//					OLED_Fill(57, 20, 76, 36, 0);
//					OLED_Show_CustomChar(57,20,(uint8_t *)icon_pause_unchecked_16_16,16,1);
//					GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//			}
//		}
//		else								//音乐停止播放时，要显示的是播放icon
//		{
//			if(key_status.if_key_mid_prsd && !key_status.if_key_mid_prsd_last)//按下中键
//			{
//					OLED_Fill(60, 20, 76, 36, 0);
//					OLED_Show_CustomChar(60,20,(uint8_t *)icon_play_checked_16_16,16,1);
//					GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//			}
//			if(!key_status.if_key_mid_prsd && key_status.if_key_mid_prsd_last)//释放中键
//			{
//					OLED_Fill(60, 20, 76, 36, 0);
//					OLED_Show_CustomChar(60,20,(uint8_t *)icon_play_unchecked_16_16,16,1);
//					GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//			}
//		}
//		
//		//“上一首”按钮
//		if(key_status.if_key_left_prsd && !key_status.if_key_left_prsd_last)
//		{
//				qcc5125_key_press(KEY_PREV, 1);
//				OLED_Show_CustomChar(24,20,(uint8_t *)icon_prev_checked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		if(!key_status.if_key_left_prsd && key_status.if_key_left_prsd_last)
//		{
//				qcc5125_key_press(KEY_PREV, 0);
//				OLED_Show_CustomChar(24,20,(uint8_t *)icon_prev_checked_16_16,16,0);
//				OLED_Show_CustomChar(24,20,(uint8_t *)icon_prev_unchecked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		//“下一首”按钮
//		if(key_status.if_key_right_prsd && !key_status.if_key_right_prsd_last)
//		{
//				qcc5125_key_press(KEY_NEXT, 1);
//				OLED_Show_CustomChar(88,20,(uint8_t *)icon_next_checked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		if(!key_status.if_key_right_prsd && key_status.if_key_right_prsd_last)
//		{
//				qcc5125_key_press(KEY_NEXT, 0);
//				OLED_Show_CustomChar(88,20,(uint8_t *)icon_next_checked_16_16,16,0);
//				OLED_Show_CustomChar(88,20,(uint8_t *)icon_next_unchecked_16_16,16,1);
//				GUI_OLEDRFS_REQ = 1; //刷新屏幕请求
//		}
//		
//		//记录上一次键值
//		bsp_key_record(&key_status);
}


//eq页面UI
//各段eq对应线段x坐标：
//				case eq_63:		10
//						break;
//				case eq_125:		22
//						break;
//				case eq_250:		34
//						break;
//				case eq_500:		46
//						break;
//				case eq_1000:		58
//						break;
//				case eq_2000:		70
//						break;
//				case eq_4000:		82
//						break;
//				case eq_8000:		94
//						break;
//				case eq_12000:		106
//						break;
//				case eq_16000:		118
//						break;
//各段eq对应线段y坐标介于10-63
void GUI_eq(int8_t *eq_status)
{		
//		OLED_Show_CustomChar(64, 0, (uint8_t *)icon_triangle_left_8_8, 8, 1);
//		OLED_ShowString(88, 57, (uint8_t *)"63", 8, 1);
//		OLED_Show_CustomChar(115, 0, (uint8_t *)icon_triangle_right_8_8, 8, 1);
//	
		//更新显示的boost值
		gui_update_eq_boost();

		//底部分界线 
		OLED_DrawLine(8, 9, 120, 9, 1);
		
		uint8_t y_coor[2] = {0};
		for(eq_chnl_e i=eq_63; i<=eq_16000; i++)
		{
				//画上新点
				y_coor[0] = (eq_status[i]+EQ_BOOST_MAX)*53.0/(2*EQ_BOOST_MAX) + 9;
				if(y_coor[0] > 62)
						y_coor[0] = 62;
				else if(y_coor[0] < 11)
						y_coor[0] = 11;
				OLED_Draw_Point_3_3(eq_gui_x_coor[i], y_coor[0], 1);
		}
		//画指示线
		OLED_DrawLine(eq_gui_x_coor[select_eq], 11, eq_gui_x_coor[select_eq], 62, 1);
}

//画gui eq界面eq的曲线，如果没有变化就不画
//eq_status为介于-12~12之间的浮点数
void GUI_oled_draw_wave(int8_t *eq_status, int8_t *eq_status_last)
{
		uint8_t y_coor = 0;
		for(eq_chnl_e i=eq_63; i<=eq_16000; i++)
		{
				//当eq值发生改变
				if(eq_status[i] != eq_status_last[i])
				{
						//更新eq设定
						cw2015_iic_musk = 1;//暂停电量计定时通信
						bsp_adau1761_eq_set(i, eq_setting_boost[i]);
						bsp_adau1761_eq_data_conv(&DSP_EQ[i]);
						bsp_adau1761_eq_download(i);
						bsp_tim22_delay_us(10);
						cw2015_iic_musk = 0;
						//擦除旧点
						OLED_DrawLine(eq_gui_x_coor[i], 10, eq_gui_x_coor[i], 63, 0);
						OLED_DrawLine(eq_gui_x_coor[i]-1, 10, eq_gui_x_coor[i]-1, 63, 0);
						OLED_DrawLine(eq_gui_x_coor[i]+1, 10, eq_gui_x_coor[i]+1, 63, 0);
						//画上新点
						y_coor = (eq_status[i]+EQ_BOOST_MAX)*53.0/(2*EQ_BOOST_MAX) + 9;
						if(y_coor > 62)
								y_coor= 62;
						else if(y_coor < 11)
								y_coor = 11;
						//画指示线
						OLED_DrawLine(eq_gui_x_coor[i], 11, eq_gui_x_coor[i], 62, 1);
						OLED_Draw_Point_3_3(eq_gui_x_coor[i], y_coor, 1);
	
						//底部显示对应eq段的boost
						OLED_DrawLine(8, 9, 120, 9, 1);
						gui_update_eq_boost();
				}
		}
}

//更新底部eqboost数值
void gui_update_eq_boost(void)
{
		OLED_ShowString(8, 57, (uint8_t *)"           ", 8, 1);
		OLED_ShowInt(8, 57, eq_setting_boost[select_eq], 8, 1);
}


//更新eq界面中心频率指示线
void gui_update_eq_line(int8_t *eq_status, eq_chnl_e current_channel, eq_chnl_e last_channel)
{
		uint8_t y_coor = 0;
		//擦除旧线
		OLED_DrawLine(eq_gui_x_coor[last_channel], 11, eq_gui_x_coor[last_channel], 62, 0);
		//画新线
		OLED_DrawLine(eq_gui_x_coor[current_channel], 11, eq_gui_x_coor[current_channel], 62, 1);
		//补画上旧点
		y_coor = (eq_status[last_channel]+EQ_BOOST_MAX)*53.0/(2*EQ_BOOST_MAX) + 9;
		if(y_coor > 62)
				y_coor = 62;
		else if(y_coor < 11)
				y_coor = 11;
		OLED_Draw_Point_3_3(eq_gui_x_coor[last_channel], y_coor, 1);
}


//更改显示在eq页面下面的当前eq中心频率
void gui_update_eq_freq(eq_chnl_e current_channel, eq_chnl_e last_channel)
{
		if(current_channel == last_channel)
				return;
		//更新现实的boost值
		gui_update_eq_boost();
		//更新指示线
		gui_update_eq_line(eq_setting_boost, current_channel, last_channel);
		switch(current_channel)
		{
				case eq_63:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(90, 57, (uint8_t *)"63", 8, 1);
						break;
				case eq_125:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(86, 57, (uint8_t *)"125", 8, 1);
						break;
				case eq_250:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(86, 57, (uint8_t *)"250", 8, 1);
						break;
				case eq_500:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(86, 57, (uint8_t *)"500", 8, 1);
						break;
				case eq_1000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(90, 57, (uint8_t *)"1k", 8, 1);
						break;
				case eq_2000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(90, 57, (uint8_t *)"2k", 8, 1);
						break;
				case eq_4000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(90, 57, (uint8_t *)"4k", 8, 1);
						break;
				case eq_8000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(90, 57, (uint8_t *)"8k", 8, 1);
						break;
				case eq_12000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(86, 57, (uint8_t *)"12k", 8, 1);
						break;
				case eq_16000:
						OLED_ShowString(84, 57, (uint8_t *)"     ", 8, 1);
						OLED_ShowString(86, 57, (uint8_t *)"16k", 8, 1);
						break;
		}
}


