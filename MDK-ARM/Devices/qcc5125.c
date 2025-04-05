#include "qcc5125.h" 
#include "cs43131.h" 
#include "audio.h"
#include "sys_init.h"
#include "tim.h"
#include "main.h"


ioctrl_t 					qcc5125_btn[4];
qcc5125_status_t  qcc5125_status;



static void qcc5125_ioctl(QCC5125_vbtn_e key, uint8_t level){
		switch(key){
			case KEY_PLAY:
				if(level)
						BL_PLAY_PRESS;
				else
						BL_PLAY_RELEASE;
				break;
					
			case KEY_PREV:
				if(level)
						BL_PREV_PRESS;
				else
						BL_PREV_RELEASE;
				break;
					
			case KEY_NEXT:
				if(level)
						BL_NEXT_PRESS;
				else
						BL_NEXT_RELEASE;
				break;
					
			case KEY_PWR:
				if(level)
						BL_PWR_PRESS;
				else
						BL_PWR_RELEASE;
				break;
		}
}


// 按下qcc5125的按键
// btn：要按下的按键 time：需要按下的时间(ms)，到时间自动释放
// 延时分辨率取决于io_proc的周期
void qcc5125_btn_press(QCC5125_vbtn_e btn, uint16_t time)
{
		//先确保松开按键
		qcc5125_btn[btn].mode = 0;
		//设置按下时间并清空计时器
		qcc5125_btn[btn].delay_set = time;
		qcc5125_btn[btn].delay_cnt = 0;
		//开始轮询buz
		qcc5125_btn[btn].mode = 1;
}



// 更新qcc5125的状态，包括io、连接/播放状态等
// time:被调用的周期，以ms计算
void qcc5125_status_proc(qcc5125_status_t *stat, uint8_t time)
{
		// 更新虚拟按键
		for(QCC5125_vbtn_e i = KEY_PLAY; i <= KEY_PWR; i++){
			
			if(qcc5125_btn[i].mode == 0)
					qcc5125_ioctl(i, 0);
			else if(qcc5125_btn[i].mode == 1){
				if(qcc5125_btn[i].delay_cnt * time < qcc5125_btn[i].delay_set){
						qcc5125_ioctl(i, 1);
						qcc5125_btn[i].delay_cnt++;
				}
				else
				{
						qcc5125_ioctl(i, 0);
						qcc5125_btn[i].mode = 0;
				}
			}
		}
		// 记录上一次的模块状态
		stat->is_connected_last = qcc5125_status.is_connected;
		stat->is_music_playing_last = qcc5125_status.is_music_playing;
		// 更新模块输出引脚状态
		stat->is_connected = IF_BL_CNED;
		stat->is_music_playing = IF_MUSIC_PLAYING;
}



