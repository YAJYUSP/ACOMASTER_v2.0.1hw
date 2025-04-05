#ifndef  __QCC5125_H
#define __QCC5125_H

#include "stdint.h"

/* QCC5125 INPUT*/
#define IF_MUSIC_PLAYING  HAL_GPIO_ReadPin(BL_AUEN_GPIO_Port, BL_AUEN_Pin)
#define IF_BL_CNED  HAL_GPIO_ReadPin(BL_CNED_GPIO_Port, BL_CNED_Pin)

/* QCC5125 OUTPUT*/
#define BL_PLAY_PRESS  		HAL_GPIO_WritePin(BL_PLAY_GPIO_Port, BL_PLAY_Pin, GPIO_PIN_SET)
#define BL_PREV_PRESS  		HAL_GPIO_WritePin(BL_PREV_GPIO_Port, BL_PREV_Pin, GPIO_PIN_SET)
#define BL_NEXT_PRESS  		HAL_GPIO_WritePin(BL_NEXT_GPIO_Port, BL_NEXT_Pin, GPIO_PIN_SET)
#define BL_PWR_PRESS  		HAL_GPIO_WritePin(BL_PWR_GPIO_Port, BL_PWR_Pin, GPIO_PIN_SET)
#define BL_PLAY_RELEASE   HAL_GPIO_WritePin(BL_PLAY_GPIO_Port, BL_PLAY_Pin, GPIO_PIN_RESET)
#define BL_PREV_RELEASE   HAL_GPIO_WritePin(BL_PREV_GPIO_Port, BL_PREV_Pin, GPIO_PIN_RESET)
#define BL_NEXT_RELEASE   HAL_GPIO_WritePin(BL_NEXT_GPIO_Port, BL_NEXT_Pin, GPIO_PIN_RESET)
#define BL_PWR_RELEASE  	HAL_GPIO_WritePin(BL_PWR_GPIO_Port, BL_PWR_Pin, GPIO_PIN_RESET)



//需要主控操作的qcc5125的几个按键
typedef enum
{
		KEY_PLAY = 0,
		KEY_PREV = 1,
		KEY_NEXT = 2,
		KEY_PWR = 3
}QCC5125_vbtn_e;


// IO控制用状态机结构体
typedef struct 
{
		// 用于控制脉冲高低电平时间
		uint16_t delay_cnt;
		uint16_t delay_set;
	
		uint8_t pulse_cnt;
		uint8_t on_set;
		uint8_t off_set;
		uint8_t pulse_set;
		//0：长关
		//1：常开
		//2：脉冲
		uint8_t mode;
}ioctrl_t;


// qcc5125的输出信号状态
typedef struct 
{
		// 正在播放音乐
		uint8_t is_music_playing, is_music_playing_last;
		// 连接成功
		uint8_t is_connected, is_connected_last;
}qcc5125_status_t;


extern qcc5125_status_t qcc5125_status;


void qcc5125_btn_press(QCC5125_vbtn_e btn, uint16_t time);
void qcc5125_status_proc(qcc5125_status_t *stat, uint8_t time);


#endif
















