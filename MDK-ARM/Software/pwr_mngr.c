#include "main.h"
#include "led.h"
#include "pwr_mngr.h"
#include "cw2015.h" 
#include "oled.h"
#include "gui.h"
#include "tim.h"
#include "cs43131.h" 
#include "key.h"
#include "qcc5125.h" 
#include "rtc.h"



//系统电量信息结构体
sys_chg_t sys_chg_info;
//系统控制信息结构体
sys_ctrl_t sys_ctrl;


//系统工作状态自动切换
void sys_ctrl_status_switch(sys_ctrl_t *ctrl)
{
		//息屏动作
		if(ctrl->sys_status == SCREEN_OFF && ctrl->sys_status_last != SCREEN_OFF)
		{
				//关闭屏幕显示
				HAL_Delay(100);
				OLED_displayOFF();
				key_timeout_status.key_prsd_flag = 0;
				
				//息屏前先打开RTC中断，用于自动关机检测
				HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 10, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
				
				HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);
		}
		
		//关机动作
		if(ctrl->sys_status == POWER_OFF && ctrl->sys_status_last != POWER_OFF)
		{
				//关闭RTC唤醒中断
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
				//关定时器中断
				HAL_TIM_Base_Stop_IT(&htim21);
				HAL_TIM_Base_Stop_IT(&htim6);
				HAL_Delay(100);
				//系统掉电
				sys_pwrctrl_power_down();
		}
		ctrl->sys_status_last = ctrl->sys_status;
}


//无音频自动关机的超时次数
uint16_t auto_pwroff_timeout = 0;
//RTC唤醒中断回调，只有在设备息屏前才使能该中断回调
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
		//重新配置系统时钟
		HAL_Init();
		SystemClock_Config();
	
		if(!IF_MUSIC_PLAYING)
			auto_pwroff_timeout ++;	
		else
			auto_pwroff_timeout = 0;
		
		if(auto_pwroff_timeout >= DEVICE_PWROFF_TIMEOUT_TIME)
		{
				//到达了设定的自动关机时间
				sys_ctrl.sys_status = POWER_OFF;
				//重新配置系统时钟
				HAL_Init();
				SystemClock_Config();
				//关闭RTC唤醒中断
				HAL_RTCEx_DeactivateWakeUpTimer(hrtc);
				//清零自动关机计数器
				auto_pwroff_timeout = 0;
				HAL_Delay(100);
				//开启屏幕显示
				OLED_displayON();
				HAL_Delay(300);
		}
		//继续睡觉
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);
}





//系统开机，应参照datasheet 中的 power up sequencing:
//cs43131:
//VP must be powered up first. 
//All other supplies can come up in any order before RESET is released. 
void sys_pwrctrl_power_up(void)
{
		//开启CODEC_VP(3v3)供电
		HAL_GPIO_WritePin(SYS_CODEC_PWR_GPIO_Port, SYS_CODEC_PWR_Pin, GPIO_PIN_SET);
		HAL_Delay(5);
		//全局复位2ms
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_SET);
		HAL_Delay(2);
		//开启全局供电
		HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_SET);
		HAL_Delay(5);
		//全局复位2ms
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_SET);
		HAL_Delay(2);
}

//LOAD %L INCREMENTAL
//系统关机，应参照datasheet 中的 power down sequencing
void sys_pwrctrl_power_down(void)
{
		//全局复位
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		//关闭CODEC1v8供电
		HAL_GPIO_WritePin(SYS_CODEC_PWR_GPIO_Port, SYS_CODEC_PWR_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		//关闭主3v3供电
		HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_RESET);	
		HAL_Delay(2);
		//取消复位
		HAL_GPIO_WritePin(SYS_RST_N_GPIO_Port, SYS_RST_N_Pin, GPIO_PIN_SET);
		//关闭电量计
		bsp_cw2015_slp(0);
		// 设置所有GPIO模式为浮空模式
		Configure_GPIO_Floating();
		HAL_Delay(2);
		//使能WAKEUP引脚(PA0)
		HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
		//清理唤醒标志 防止立刻唤醒
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	  
		HAL_PWR_EnterSTANDBYMode();
}

// 设置所有GPIO模式为浮空模式
void Configure_GPIO_Floating(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;   
	
		GPIO_InitStruct.Pin = KEY_UP_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(KEY_UP_GPIO_Port, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = KEY_DOWN_Pin;
		HAL_GPIO_Init(KEY_DOWN_GPIO_Port, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin = KEY_LEFT_Pin;
		HAL_GPIO_Init(KEY_LEFT_GPIO_Port, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = KEY_RIGHT_Pin;
		HAL_GPIO_Init(KEY_RIGHT_GPIO_Port, &GPIO_InitStruct);
}

//初始化系统电量信息 
void sys_chg_info_init(sys_chg_t *chg_info)
{
		sys_chg_updt_info(chg_info);
		HAL_Delay(5);
}

//更新系统电量信息
void sys_chg_updt_info(sys_chg_t *chg_info)
{
		//电池电压
//		chg_info->battery_volt = bsp_cw2015_read_volt();
//		if(chg_info->battery_volt > 4200)		
//				chg_info->battery_volt = 4200;
//		else if(chg_info->battery_volt < 3000)		
//				chg_info->battery_volt = 3000;
//		chg_info->soc = chg_info->battery_volt * 100/1200;
		
//		//电量百分比
//		chg_info->chg_prctg = bsp_cw2015_read_soc(0);
//		if(chg_info->chg_prctg > 100)
//				chg_info->chg_prctg = 100;
		//剩余使用时间
//		chg_info->remain_rt = bsp_cw2015_get_time();
}
