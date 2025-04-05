#include "main.h"
#include "audio.h"
#include "adau1761.h"
#include "cs43131.h"
#include "qcc5125.h"
 
//audio_ctrl_t  globalAudio;

// 音频链路使能标志位，在每次开机之后，只进行一次音频链路初始化，且在第一次出现MCLK后进行 
// 为0代表没有进行过初始化，为1代表进行过
uint8_t audio_init_flag = 0;




//音频链路暂停，将会按下qcc5125的暂停键
static void audio_pulse(void)
{
		//暂停音乐
		qcc5125_btn_press(KEY_PLAY, 200);
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
					//配置adau1761
					default_download_IC_2();
					//等待adau1761工作稳定
					HAL_Delay(1);
					//加载eq
					for(eq_chnl_e i=eq_63; i<=eq_16000; i++)
					{
							bsp_adau1761_eq_set(i, eq_setting_boost[i]);
							bsp_adau1761_eq_data_conv(&DSP_EQ[i]);
							bsp_adau1761_eq_download(i);
					}
					HAL_Delay(1);
					//配置cs43131
					bsp_43131_init();
					//设置cs43131音量
					audio_unmute();
				}
		}
}



