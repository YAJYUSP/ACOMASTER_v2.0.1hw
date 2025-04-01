#include "main.h"
#include "audio.h"
#include "adau1761.h"
#include "cs43131.h"
#include "qcc5125.h"
 
 //音频链路使能标志位，在每次开机之后，只进行一次音频链路初始化，且在第一次出现MCLK后进行 
//为0代表没有进行过初始化，为1代表进行过
uint8_t audio_init_flag = 0;

//音频链路暂停，将会静音codec并按下qcc5125的暂停键
void audio_pulse(void)
{
		//静音CODEC
		bsp_43131_set_volume(0);
		//暂停音乐
		qcc5125_btn_press(KEY_PLAY, 2);
}


void audio_mute(void)
{
		//静音CODEC
		bsp_43131_set_volume(0);
}
void audio_unmute(void)
{
		//设置cs43131音量
		cs43131_volume = 85;
		bsp_43131_set_volume(cs43131_volume);
}


//音频链路使能，在MCLK刚刚出现（音乐刚刚播放）时调用，将会初始化所有音频外设
void audio_enable(void)
{
		//配置adau1761
		default_download_IC_2();
		//等待adau1761工作稳定
		HAL_Delay(10);
		//加载eq
		for(eq_chnl_e i=eq_63; i<=eq_16000; i++)
		{
				bsp_adau1761_eq_set(i, eq_setting_boost[i]);
				bsp_adau1761_eq_data_conv(&DSP_EQ[i]);
				bsp_adau1761_eq_download(i);
		}
		HAL_Delay(10);
		//配置cs43131
		bsp_43131_init();
		//设置cs43131音量
		audio_unmute();
}


