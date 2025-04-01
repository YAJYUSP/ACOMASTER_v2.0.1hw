#ifndef   _LED_H_
#define  _LED_H_

typedef enum
{
		LED_ON,
		LED_OFF,
		LED_TOGGLE,
}LED_State;

void bsp_led_ctrl(LED_State sta);


#endif
