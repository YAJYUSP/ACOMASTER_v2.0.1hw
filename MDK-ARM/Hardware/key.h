#ifndef   _KEY_H_
#define  _KEY_H_

#include "main.h"

#define IF_KEY_MID_PRSD  					HAL_GPIO_ReadPin(KEY_MID_GPIO_Port, KEY_MID_Pin)
#define IF_KEY_LEFT_PRSD  				!HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin)
#define IF_KEY_UP_PRSD  					!HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
#define IF_KEY_RIGHT_PRSD   			!HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin)
#define IF_KEY_DOWN_PRSD  				!HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin)

#define IF_CHARGING 							!HAL_GPIO_ReadPin(CHARGING_GPIO_Port, CHARGING_Pin)
#define IF_CHARGE_DONE						!HAL_GPIO_ReadPin(CHRG_DONE_GPIO_Port, CHRG_DONE_Pin)

void bsp_btn_scan_init(void);



#endif
