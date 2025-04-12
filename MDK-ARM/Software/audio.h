#ifndef   _AUDIO_H_
#define  _AUDIO_H_

#include "qcc5125.h"
#include "adau1761.h"

typedef struct audio_config_t { 	// 和音频播放有关的配置项
  uint8_t selected_preset; 				// 当前选择的预设，置0-3分别为Normal、1、2、3
	uint8_t hp_detecting;						// 耳机插拔检测，0为不进行检测
	int8_t preset_eq[4][10];				// 用于存储四个预设的十段eq值
}audio_config_t;  

// qcc5125的输出信号状态
typedef struct 
{
		// 正在播放音乐
		uint8_t is_music_playing, is_music_playing_last;
		// 连接成功
		uint8_t is_connected, is_connected_last;
}audio_status_t;


void audio_pulse(void);
void audio_prev(void);
void audio_next(void);
void audio_volp(void);
void audio_vols(void);

void audio_status_proc(audio_status_t *stat, uint8_t time);
void audio_download_eqchnl(audio_config_t *conf, eq_cfreq_e cfreq);
void audio_param_init(eq_param_t *eq, audio_config_t* cfg);

void audio_save_configs_to_eeprom(audio_config_t *cfg);
void audio_read_configs_from_eeprom(audio_config_t *cfg);

#endif
