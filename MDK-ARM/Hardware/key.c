/*
 * 此源文件用于实现所有按键事件与MCU外部GPIO输入处理
 * 搭配multibutton库使用
 * 
 */

#include "main.h"
#include "key.h"
#include "multi_button.h"

#include "WouoUI.h"


/* 注册按键结构体*/
struct Button btn_up, btn_down, btn_prev, btn_next, btn_mid;

/* 注册按键对应id*/
const uint8_t btn_up_id   = 0;
const uint8_t btn_down_id = 1;
const uint8_t btn_prev_id = 2;
const uint8_t btn_next_id = 3;
const uint8_t btn_mid_id  = 4;


/* 绑定按键id与其对应的电平读取函数*/
uint8_t read_btn_level(uint8_t btn_id)
{
	switch(btn_id)
	{
		case btn_up_id:
			return !HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
			break;
		
		case btn_down_id:
			return !HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
			break;
		
		case btn_prev_id:
			return !HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin);
			break;
		
		case btn_next_id:
			return !HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin);
			break;
		
		case btn_mid_id:
			return HAL_GPIO_ReadPin(KEY_MID_GPIO_Port, KEY_MID_Pin);
			break;
		
		default:
			break;
	}
}


/* 按键单击事件回调*/
void BTNUP_SINGLE_Click_Handler(void* btn)
{
	OLED_MsgQueSend(msg_add);
}

void BTNDOWN_SINGLE_Click_Handler(void* btn)
{
	OLED_MsgQueSend(msg_sub);
}

void BTNPREV_SINGLE_Click_Handler(void* btn)
{
	OLED_MsgQueSend(msg_up);
}

void BTNNEXT_SINGLE_Click_Handler(void* btn)
{
	OLED_MsgQueSend(msg_down);
}

void BTNMID_SINGLE_Click_Handler(void* btn)
{
	OLED_MsgQueSend(msg_click);
}



/* 按键长按事件回调*/
void BTNPREV_LONG_PRESS_START_Handler(void* btn)
{
	OLED_MsgQueSend(msg_return);
}

void BTNMID_LONG_PRESS_START_Handler(void* btn)
{
	
}



/* 初始化按键扫描服务*/
void bsp_btn_scan_init(void)
{
		button_init(&btn_up, read_btn_level, 1, btn_up_id);
		button_attach(&btn_up, SINGLE_CLICK,     BTNUP_SINGLE_Click_Handler);
		button_start(&btn_up);
	
		button_init(&btn_down, read_btn_level, 1, btn_down_id);
		button_attach(&btn_down, SINGLE_CLICK,     BTNDOWN_SINGLE_Click_Handler);
		button_start(&btn_down);
	
		button_init(&btn_prev, read_btn_level, 1, btn_prev_id);
		button_attach(&btn_prev, SINGLE_CLICK,     BTNPREV_SINGLE_Click_Handler);
		button_attach(&btn_prev, LONG_PRESS_START, BTNPREV_LONG_PRESS_START_Handler);
		button_start(&btn_prev);
	
		button_init(&btn_next, read_btn_level, 1, btn_next_id);
		button_attach(&btn_next, SINGLE_CLICK,     BTNNEXT_SINGLE_Click_Handler);
		button_start(&btn_next);
	
		button_init(&btn_mid, read_btn_level, 1, btn_mid_id);
		button_attach(&btn_mid, SINGLE_CLICK,     BTNMID_SINGLE_Click_Handler);
		button_attach(&btn_mid, LONG_PRESS_START, BTNMID_LONG_PRESS_START_Handler);
		button_start(&btn_mid);
}

