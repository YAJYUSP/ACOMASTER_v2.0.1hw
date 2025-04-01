#include "qcc5125.h" 
#include "cs43131.h" 
#include "audio.h"
#include "sys_init.h"
#include "tim.h"
#include "main.h"


e_btn_t qcc5125_btn[4];
qcc5125_status_t qcc5125_status;


//time*100（ms）为虚拟按键按下时间
void qcc5125_btn_press(QCC5125_KEY key, uint8_t time)
{
		//先关闭buz
		qcc5125_btn[key].mode = 0;
		//设置鸣响时间并清空计时器
		qcc5125_btn[key].delay_set = time;
		qcc5125_btn[key].delay_cnt = 0;
		//开始轮询buz
		qcc5125_btn[key].mode = 1;
}



//更新qcc5125的io状态，包括连接/播放状态，按键控制等
void qcc5125_io_update_100ms(void)
{
		//更新虚拟按键
		for(QCC5125_KEY i=KEY_PLAY; i<=KEY_PWR; i++)
		{
				if(qcc5125_btn[i].mode == 0)
						qcc5125_key_press(i, 0);
				else if(qcc5125_btn[i].mode == 1)
				{
						if(qcc5125_btn[i].delay_cnt < qcc5125_btn[i].delay_set)
						{
								qcc5125_key_press(i, 1);
								qcc5125_btn[i].delay_cnt++;
						}
						else
						{
								qcc5125_key_press(i, 0);
								qcc5125_btn[i].mode = 0;
						}
				}
    }
		
}



void qcc5125_key_press(QCC5125_KEY key, uint8_t status)
{
		switch(key)
		{
				case KEY_PLAY:
						if(status)
								BL_PLAY_PRESS;
						else
								BL_PLAY_RELEASE;
						break;
						
				case KEY_PREV:
						if(status)
								BL_PREV_PRESS;
						else
								BL_PREV_RELEASE;
						break;
						
				case KEY_NEXT:
						if(status)
								BL_NEXT_PRESS;
						else
								BL_NEXT_RELEASE;
						break;
						
				case KEY_PWR:
						if(status)
								HAL_GPIO_WritePin(BL_PWR_GPIO_Port, BL_PWR_Pin, GPIO_PIN_SET);
						else
								HAL_GPIO_WritePin(BL_PWR_GPIO_Port, BL_PWR_Pin, GPIO_PIN_RESET);
						break;
		}
}


