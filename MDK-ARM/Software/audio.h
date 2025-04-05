#ifndef   _AUDIO_H_
#define  _AUDIO_H_

#include "qcc5125.h"

//// 音频链路控制结构体
//typedef struct 
//{
//	// 音频链路使能标志位，在每次开机之后，只进行一次音频链路初始化，且在第一次出现MCLK后进行 
//	// 为0代表没有进行过初始化，为1代表进行过
//	uint8_t audio_init_flag;
//	
//}audio_ctrl_t;

void audio_enable_proc(qcc5125_status_t qcs, uint8_t time);

#endif
