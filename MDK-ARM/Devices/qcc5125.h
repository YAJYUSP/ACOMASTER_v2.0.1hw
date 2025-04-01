#ifndef  __QCC5125_H
#define __QCC5125_H

#include "stdint.h"

#define IF_MUSIC_PLAYING  HAL_GPIO_ReadPin(BL_AUEN_GPIO_Port, BL_AUEN_Pin)
#define IF_BL_CNED  HAL_GPIO_ReadPin(BL_CNED_GPIO_Port, BL_CNED_Pin)

#define BL_PLAY_PRESS  HAL_GPIO_WritePin(BL_PLAY_GPIO_Port, BL_PLAY_Pin, GPIO_PIN_SET)
#define BL_PREV_PRESS  HAL_GPIO_WritePin(BL_PREV_GPIO_Port, BL_PREV_Pin, GPIO_PIN_SET)
#define BL_NEXT_PRESS  HAL_GPIO_WritePin(BL_NEXT_GPIO_Port, BL_NEXT_Pin, GPIO_PIN_SET)
#define BL_PLAY_RELEASE  HAL_GPIO_WritePin(BL_PLAY_GPIO_Port, BL_PLAY_Pin, GPIO_PIN_RESET)
#define BL_PREV_RELEASE   HAL_GPIO_WritePin(BL_PREV_GPIO_Port, BL_PREV_Pin, GPIO_PIN_RESET)
#define BL_NEXT_RELEASE   HAL_GPIO_WritePin(BL_NEXT_GPIO_Port, BL_NEXT_Pin, GPIO_PIN_RESET)

//qcc5125的三个虚拟按键
typedef enum
{
		KEY_PLAY = 0,
		KEY_PREV = 1,
		KEY_NEXT = 2,
		KEY_PWR = 3
}QCC5125_KEY;


//模拟qcc5125的按键操作
typedef struct 
{
		uint8_t delay_cnt;
		uint8_t pulse_cnt;
		//用于控制脉冲高低电平时间
		uint8_t delay_set;
		uint8_t on_set;
		uint8_t off_set;
		uint8_t pulse_set;
		//0：长关
		//1：常开
		//2：脉冲
		uint8_t mode;
}e_btn_t;


//qcc5125的输出信号状态
typedef struct 
{
	
		//codec mute延时
		uint8_t mute_delay_cnt;
		//codec mute延时使能，为1开始计时
		uint8_t mute_delay_enable;
		//正在播放音乐
		uint8_t is_music_playing, is_music_playing_last;
		//连接成功
		uint8_t is_connected, is_connected_last;
		//输出的音频采样时钟频率
		float freq_i2s_lrck;
		//圆整后得到的音频采样率
		float audio_sample_rate;

}qcc5125_status_t;


extern qcc5125_status_t qcc5125_status;


void qcc5125_btn_press(QCC5125_KEY key, uint8_t time);
void qcc5125_io_update_100ms(void);
void qcc5125_key_press(QCC5125_KEY key, uint8_t status);

#endif
















