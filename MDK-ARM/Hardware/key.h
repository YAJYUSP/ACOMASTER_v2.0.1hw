#ifndef   _KEY_H_
#define  _KEY_H_

#include "main.h"

#define IF_KEY_MID_PRSD  				HAL_GPIO_ReadPin(KEY_MID_GPIO_Port, KEY_MID_Pin)
#define IF_KEY_LEFT_PRSD  				!HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin)
#define IF_KEY_UP_PRSD  					!HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
#define IF_KEY_RIGHT_PRSD   		!HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin)
#define IF_KEY_DOWN_PRSD  		!HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin)

#define IF_CHARGING 								!HAL_GPIO_ReadPin(CHARGING_GPIO_Port, CHARGING_Pin)
#define IF_CHARGE_DONE					!HAL_GPIO_ReadPin(CHRG_DONE_GPIO_Port, CHRG_DONE_Pin)


typedef struct
{
		//按键未被触发的超时时间，超时后将进入息屏模式
		uint16_t key_timeout_not_prsd;
		//任意按键被触发的flag，用于判断上述超时时间
		uint8_t key_prsd_flag;
}key_timeout_status_t;


typedef struct
{
		//按键被触发的时间，超时将判定为长按
		uint16_t key_longpress_time;
		//按键被判定为长按的flag
		uint8_t key_long_prsd_flag;
}key_longpress_t;

typedef struct
{
		uint8_t if_key_up_prsd, if_key_up_prsd_last;
		uint8_t if_key_down_prsd, if_key_down_prsd_last;
		uint8_t if_key_left_prsd, if_key_left_prsd_last;
		uint8_t if_key_right_prsd, if_key_right_prsd_last;
		uint8_t if_key_mid_prsd, if_key_mid_prsd_last;
}key_status_t;

extern key_timeout_status_t key_timeout_status;
extern uint16_t key_middle_prsd_time;
extern key_status_t key_status;
extern key_longpress_t key_longpress_up, key_longpress_down;

void bsp_key_scan(key_status_t *key);
void bsp_key_record(key_status_t *key);
void key_longpress_scan_5ms(void);

#endif
