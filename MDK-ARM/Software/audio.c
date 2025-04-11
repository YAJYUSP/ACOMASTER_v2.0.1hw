#include "main.h"
#include "audio.h"
#include "adau1761.h"
#include "cs43131.h"
#include "qcc5125.h"
#include "eeprom.h"
 

// 音频链路使能标志位，在每次开机之后，只进行一次音频链路初始化，且在第一次出现MCLK后进行 
// 为0代表没有进行过初始化，为1代表进行过
uint8_t audio_init_flag = 0;

audio_config_t configures;


// 音频链路暂停，将会按下qcc5125的暂停键
void audio_pulse(void)
{
		qcc5125_btn_press(KEY_PLAY, 200);
}
// 上一首
void audio_prev(void)
{
		qcc5125_btn_press(KEY_PREV, 200);
}
// 下一首
void audio_next(void)
{
		qcc5125_btn_press(KEY_NEXT, 200);
}
// 音量+
void audio_volp(void)
{
		qcc5125_btn_press(KEY_NEXT, 550);
}
// 音量-
void audio_vols(void)
{
		qcc5125_btn_press(KEY_PREV, 550);
}



static void audio_mute(void)
{
		//静音CODEC
		bsp_43131_set_volume(0);
}
static void audio_unmute(void)
{
		//设置cs43131音量
		bsp_43131_set_volume(85);
}


/* 配置某eq通道的boost值进DSP*/
void audio_download_eqchnl(eq_param_t *eq)
{
	bsp_adau1761_eq_download(eq);
}


extern eq_param_t eq_channel[10];  // 10段EQ结构体的数组

// 音频链路使能，在MCLK刚刚出现（音乐刚刚播放）时会初始化所有音频外设
void audio_enable_proc(qcc5125_status_t qcs, uint8_t time)
{
		// 当模块开始播放，说明MCLK时钟由失效变为有效，此时给DSP以及CODEC进行配置
		if(qcs.is_music_playing == 1 && qcs.is_music_playing_last == 0){
				// 若没初始化过，则进行初始化并修改标志位，每次复位后最多执行一次
				if(!audio_init_flag){
						audio_init_flag = 1;
					
					// 系统静音，防止初始化的时候爆音
					audio_mute();
					HAL_Delay(1);
					// 配置adau1761
					default_download_IC_2();
					// 等待adau1761工作稳定
					HAL_Delay(1);
					// 加载配置项中存储的eq boost参数
					for(eq_cfreq_e i=freq_63; i<=freq_16k; i++)
					{
						eq_channel[i].eq_boost = configures.preset_eq[configures.selected_preset][i];
						audio_download_eqchnl(&(eq_channel[i]));
					}
					HAL_Delay(1);
					//配置cs43131
					bsp_43131_init();
					//设置cs43131音量
					audio_unmute();
				}
		}
}


//初始化和音频配置相关的结构体
void audio_param_init(eq_param_t *eq, audio_config_t* cfg)
{
	for(eq_cfreq_e i=freq_63; i<=freq_16k; i++)
	{
			eq[i].eq_boost = 0.00; // 每段EQ的boost 
			eq[i].gain = 0.0;			 // 每段EQ的增益
			eq[i].kQ = 1.41;			 // Q值
			eq[i].prj_Fs = 96000;  // 系统采样率
	}
	eq[freq_63].center_freq = 63;
	eq[freq_125].center_freq = 125;
	eq[freq_250].center_freq = 250;
	eq[freq_500].center_freq = 500;
	eq[freq_1k].center_freq = 1000;
	eq[freq_2k].center_freq = 2000;
	eq[freq_4k].center_freq = 4000;
	eq[freq_8k].center_freq = 8000;
	eq[freq_12k].center_freq = 12000;
	eq[freq_16k].center_freq = 16000;
	
	eq[freq_63].safeload_addr = 0x0008;
	eq[freq_125].safeload_addr = 0x000D;
	eq[freq_250].safeload_addr = 0x0012;
	eq[freq_500].safeload_addr = 0x0017;
	eq[freq_1k].safeload_addr = 0x001C;
	eq[freq_2k].safeload_addr = 0x0021;
	eq[freq_4k].safeload_addr = 0x0026;
	eq[freq_8k].safeload_addr = 0x002B;
	eq[freq_12k].safeload_addr = 0x0030;
	eq[freq_16k].safeload_addr = 0x0035;
	
	cfg->selected_preset = 0;
}

///* 将eq_boost[10]存进eeprom*/
//void audio_save_all_eqchnl_to_eeprom(void)
//{
//	for(uint8_t i=0; i<=9; i++)
//	{
//			EEPROM_WriteWithCheck(i, (uint8_t *)&eq_boost[i], 1);
//	}
//}

///* 从eeprom读出eq数据存入eq_boost[10]*/
//void audio_read_all_eqchnl_from_eeprom(void)
//{
//	for(uint8_t i=0; i<=9; i++)
//	{
//			EEPROM_ReadWithCheck(i, (uint8_t *)&eq_boost[i], 1);
//	}
//}

/* 将audio_config_t结构体存进eeprom*/
void audio_save_configs_to_eeprom(audio_config_t *cfg)
{
	EEPROM_WriteWithCheck(0, (uint8_t *)cfg, sizeof(audio_config_t));
}

/* 将audio_config_t结构体从eeprom读出*/
void audio_read_configs_from_eeprom(audio_config_t *cfg)
{
	EEPROM_ReadWithCheck(0, (uint8_t *)cfg, sizeof(audio_config_t));
}


