#ifndef   _AUDIO_H_
#define  _AUDIO_H_

#include "qcc5125.h"
#include "adau1761.h"

typedef struct audio_config_t { 	// 和音频播放有关的配置项
  uint8_t selected_preset; 				// 当前选择的预设，置0-3分别为Normal、1、2、3
	uint8_t hp_detecting;						// 耳机插拔检测，0为不进行检测
	int8_t preset_eq[4][10];				// 用于存储四个预设的十段eq值
}audio_config_t;  



void audio_enable_proc(qcc5125_status_t qcs, uint8_t time);
void audio_download_eqchnl(eq_param_t *eq);
void audio_param_init(eq_param_t *eq, audio_config_t* cfg);

//void audio_save_all_eqchnl_to_eeprom(void);
//void audio_read_all_eqchnl_from_eeprom(void);

void audio_save_configs_to_eeprom(audio_config_t *cfg);
void audio_read_configs_from_eeprom(audio_config_t *cfg);

#endif
