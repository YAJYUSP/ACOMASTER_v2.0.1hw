/*
 * This is WouoUI User Defined Pages file.
 * 
 */
#include "WouoUI_udp.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"

#include "main.h"
#include "audio.h"
#include "qcc5125.h"

// 全局十段EQ boost值
extern audio_config_t configures;





const unsigned char icon_play_checked_16_16[] = 
{
0x00,0xFE,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,
0x00,0x7F,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,
};
const unsigned char icon_play_unchecked_16_16[] = 
{
0x00,0xFE,0x01,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,
0x00,0x7F,0x80,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,
};

const unsigned char icon_next_checked_16_16[] = 
{
0x00,0xFE,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x7F,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0xFF,0xFF,0xFF,0x00,0x00,
};
const unsigned char icon_next_unchecked_16_16[] = 
{
0x00,0xFE,0x01,0x01,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x7F,0x01,0xFF,0x00,0x00,
0x00,0x7F,0x80,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0xFE,0x80,0xFF,0x00,0x00,
};
const unsigned char icon_prev_checked_16_16[] = 
{
0x00,0x00,0xFF,0xFF,0xFF,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFE,0x00,
0x00,0x00,0xFF,0xFF,0xFF,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,0xFF,0x7F,0x00,
};
const unsigned char icon_prev_unchecked_16_16[] = 
{
0x00,0x00,0xFF,0x01,0x7F,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x01,0x01,0xFE,0x00,
0x00,0x00,0xFF,0x80,0xFE,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x80,0x7F,0x00,
};
const unsigned char icon_empty_16_16[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const unsigned char icon_bl_disconn_16_16[] = 
{
0x00,0x00,0x00,0x10,0x20,0x40,0x80,0xFE,0x82,0x44,0x28,0x10,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x04,0x02,0x01,0x00,0x3F,0x20,0x11,0x0A,0x04,0x00,0x00,0x00,0x00
};
const unsigned char icon_bl_conn_16_16[] = 
{
0x00,0xE0,0xC0,0x90,0x20,0x40,0x80,0xFE,0x82,0x44,0x28,0x90,0xC0,0xE0,0x00,0x00,
0x00,0x07,0x03,0x04,0x02,0x01,0x00,0x3F,0x20,0x11,0x0A,0x04,0x01,0x03,0x00,0x00,
};
const unsigned char icon_vol3_16_16[] = 
{
0xE0,0x20,0xE0,0x10,0x08,0x04,0xFC,0x00,0x40,0x90,0x20,0xC4,0x08,0xF0,0x00,0x00,
0x0F,0x08,0x0F,0x10,0x20,0x40,0x7F,0x00,0x04,0x13,0x08,0x47,0x20,0x1F,0x00,0x00,
};
const unsigned char icon_vol2_16_16[] = 
{
0xE0,0x20,0xE0,0x10,0x08,0x04,0xFC,0x00,0x40,0x90,0x20,0xC0,0x00,0x00,0x00,0x00,
0x0F,0x08,0x0F,0x10,0x20,0x40,0x7F,0x00,0x04,0x13,0x08,0x07,0x00,0x00,0x00,0x00,
};
const unsigned char icon_vol1_16_16[] = 
{
0xE0,0x20,0xE0,0x10,0x08,0x04,0xFC,0x00,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x0F,0x08,0x0F,0x10,0x20,0x40,0x7F,0x00,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
};
const unsigned char icon_vol0_16_16[] = 
{
0xE0,0x20,0xE0,0x10,0x08,0x04,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x0F,0x08,0x0F,0x10,0x20,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const unsigned char icon_pause_unchecked_16_16[] = 
{
0x00,0x00,0xFE,0x01,0x01,0x01,0xFE,0x00,0xFE,0x01,0x01,0x01,0xFE,0x00,0x00,0x00,
0x00,0x00,0x7F,0x80,0x80,0x80,0x7F,0x00,0x7F,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,
};
const unsigned char icon_pause_checked_16_16[] = 
{
0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFE,0x00,0xFE,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,
0x00,0x00,0x7F,0xFF,0xFF,0xFF,0x7F,0x00,0x7F,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,
};


//**********************每个以Page为基类的类都有对应的AnimInit、show、React方法

//--------Playing页面相关函数
void OLED_PlayingPageEnterInit(PageAddr page_addr, uint16_t time) {

}

extern qcc5125_status_t qcc5125_status;

void OLED_PlayingPageShow(PageAddr page_addr, uint16_t time) {
	
	
	OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol0_16_16, 1);
	OLED_WinDrawBMP(&w_all,56, 0, 16, 16, (uint8_t *)icon_bl_disconn_16_16, 1);
	
	OLED_WinDrawBMP(&w_all,24, 35, 16, 16, (uint8_t *)icon_prev_unchecked_16_16, 1);
	OLED_WinDrawBMP(&w_all,88, 35, 16, 16, (uint8_t *)icon_next_unchecked_16_16, 1);
	if(qcc5125_status.is_music_playing)
		OLED_WinDrawBMP(&w_all,57, 35, 16, 16, (uint8_t *)icon_pause_unchecked_16_16, 1);
	else
		OLED_WinDrawBMP(&w_all,60, 35, 16, 16, (uint8_t *)icon_play_unchecked_16_16, 1);
	
}

// 实际时间 = ICON_DISP_TIMEOUT * time
#define ICON_PLAY_TIMEOUT 10
#define ANIM_VOL_TIMEOUT  10

void OLED_PlayingPageReact(PageAddr page_addr, uint16_t time) {
	
		Page *p = (Page *)page_addr;
    PlayingPage *pp = (PlayingPage *)page_addr;
    String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead(); // 空时读出msg_none
		OLED_MsgQueClear(); 							// 这里暂时清空消息队列，可能会引发问题------------------======================================================
	
		// 此计数器标志用以控制按下PLAY等按键时屏幕三大icon的变化，标志为1代表正在计数
		static uint8_t counter[3] = {0};
		static uint8_t cnt_switch[3] = {0};
		
		// 此计数器标志用以控制按下音量键时屏幕喇叭icon的变化，标志为1代表正在计数
		static uint8_t counter_vol_anim = 0;
		/* status = 0：空；   status = 1：播放“音量+”动画；   status = 2：播放“音量-”动画；*/
		static uint8_t counter_vol_status = 0;
		
		
		if(msg == msg_none){
		}
		else if(msg == msg_add){
			if(!counter_vol_status){ // 播放“音量+”动画
				counter_vol_status = 1;
			}
			if (p->cb != NULL)			 // 按键信息到来时，触发回调
					p->cb(p, &(pp->option_array[3]));
		}
		else if(msg == msg_sub){
			if(!counter_vol_status){ // 播放“音量-”动画
				counter_vol_status = 2;
			}
			if (p->cb != NULL)
					p->cb(p, &(pp->option_array[4]));
		}
		else if(msg == msg_up){    //prev
			cnt_switch[0] = 1;		
			if (p->cb != NULL)
				p->cb(p, &(pp->option_array[0]));
		}
		else if(msg == msg_down){  //next
			cnt_switch[1] = 1;
			if (p->cb != NULL)
				p->cb(p, &(pp->option_array[1]));
		}
		else if(msg == msg_click){ //play
			cnt_switch[2] = 1;
			if (p->cb != NULL)
				p->cb(p, &(pp->option_array[2]));
		}
		else if(msg == msg_return){ // 返回
			if (p->cb != NULL)
				p->cb(p, &(pp->option_array[5]));
		}
		
		/* 处理屏幕三大icon的变化*/
		if(cnt_switch[0]) {
				if(counter[0] < ICON_PLAY_TIMEOUT)
						counter[0] ++;
				else{ 
						counter[0] = 0;
						cnt_switch[0] = 0;
				}
				OLED_WinDrawBMP(&w_all,24, 35, 16, 16, (uint8_t *)icon_prev_checked_16_16, 1);
		}
		if(cnt_switch[1]) {
			if(counter[1] < ICON_PLAY_TIMEOUT)
					counter[1] ++;
			else{
					counter[1] = 0;
					cnt_switch[1] = 0;
		  }
			OLED_WinDrawBMP(&w_all,88, 35, 16, 16, (uint8_t *)icon_next_checked_16_16, 1);
		}
		if(cnt_switch[2]) {
			if(counter[2] < ICON_PLAY_TIMEOUT)
					counter[2] ++;
			else{
					counter[2] = 0;
					cnt_switch[2] = 0;
			}
			if(qcc5125_status.is_music_playing)
				OLED_WinDrawBMP(&w_all,57, 35, 16, 16, (uint8_t *)icon_pause_checked_16_16, 1);
			else
				OLED_WinDrawBMP(&w_all,60, 35, 16, 16, (uint8_t *)icon_play_checked_16_16, 1);		
		}
			
		/* 处理音量动画的变化*/
		if(counter_vol_status == 1) {
			if(counter_vol_anim < ANIM_VOL_TIMEOUT)
					counter_vol_anim ++;
			else{ 
					counter_vol_anim = 0;
					counter_vol_status = 0;
			}
		}
		else if(counter_vol_status == 2) {
			if(counter_vol_anim == 0)
				counter_vol_anim = ANIM_VOL_TIMEOUT; // 递减计数
			if(counter_vol_anim > 0)
					counter_vol_anim --;
			if(counter_vol_anim == 0)
					counter_vol_status = 0;
		}
		if(counter_vol_anim == 0)
			OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol3_16_16, 1);
		else if(counter_vol_anim < ANIM_VOL_TIMEOUT/4)
			OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol0_16_16, 1);
		else if(counter_vol_anim < ANIM_VOL_TIMEOUT/2)
			OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol1_16_16, 1);
		else if(counter_vol_anim < ANIM_VOL_TIMEOUT*3/4)
			OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol2_16_16, 1);
		else if(counter_vol_anim <= ANIM_VOL_TIMEOUT)
			OLED_WinDrawBMP(&w_all,0, 0, 16, 16, (uint8_t *)icon_vol3_16_16, 1);
		
		/* 处理蓝牙图标*/
		if(qcc5125_status.is_connected)
			OLED_WinDrawBMP(&w_all,56, 0, 16, 16, (uint8_t *)icon_bl_conn_16_16, 1);
		else
			OLED_WinDrawBMP(&w_all,56, 0, 16, 16, (uint8_t *)icon_bl_disconn_16_16, 1);
}
	
void OLED_PlayingPageInit(
    PlayingPage *playing_page,  // 磁贴页面对象
    uint8_t item_num,       // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    Icon *icon_array,       // 整个页面的icon数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
{
		playing_page->page.page_type = type_userdef; //定义页面类型为"用户自定义"
    OLED_PageInit((PageAddr)playing_page, call_back);
    playing_page->page.init = OLED_PlayingPageEnterInit;
    playing_page->page.show = OLED_PlayingPageShow;
    playing_page->page.react = OLED_PlayingPageReact; // 关联处理函数(方法)
		playing_page->select_item = 0;
    playing_page->item_num = item_num;
		playing_page->option_array = option_array;
    playing_page->icon_array = icon_array;
    for (uint8_t i = 0; i < playing_page->item_num; i++)
        playing_page->option_array[i].order = i; // 选项序号标号
}



/*----------------------------------------------------------------------------------------------------------*/




// EQ UI中不同中心频率对应的X坐标
#define FREQ_63_X_COOR 10
#define FREQ_125_X_COOR 22
#define FREQ_250_X_COOR 34
#define FREQ_500_X_COOR 46
#define FREQ_1K_X_COOR 58
#define FREQ_2K_X_COOR 70
#define FREQ_4K_X_COOR 82
#define FREQ_8K_X_COOR 94
#define FREQ_12K_X_COOR 106
#define FREQ_16K_X_COOR 118

// 计算不同boost值对应的屏幕Y坐标
static uint8_t calc_ycoor_from_boost(int8_t boost){
	return (EQ_BOOST_MAX - boost) * 54 / (2 * EQ_BOOST_MAX);
}

/**
 * @brief 插值并绘制光滑曲线
 * @param input 输入10个Y坐标（对应X=10,22,...,118）
 * @param color 绘制颜色（true为描点，false为擦除）
 */
void OLED_DrawSmoothCurve(uint8_t* input, bool color) {
    // 定义插值参数
    const uint8_t seg_points = 12;
    const uint8_t total_seg = 9;
    uint8_t x_start = 10; // 起始X坐标
    
    // 存储前一个点坐标
    int16_t prev_x = x_start;
    int16_t prev_y = input[0];
    
    // 遍历所有段
    for (uint8_t seg = 0; seg < total_seg; seg++) {
        // 控制点处理
        uint8_t p0 = (seg == 0) ? input[0] : input[seg-1];
        uint8_t p1 = input[seg];
        uint8_t p2 = input[seg+1];
        uint8_t p3 = (seg >= total_seg-2) ? input[total_seg] : input[seg+2];
        
        // 系数计算（Q4.4优化）
        int16_t a = ((-p0 + 3*p1 - 3*p2 + p3) << 2);
        int16_t b = (2*p0 - 5*p1 + 4*p2 - p3) << 2;
        int16_t c = (-p0 + p2) << 3;
        int16_t d = p1 << 4;
        
        // 生成并绘制本段曲线
        for (uint8_t t = 0; t < seg_points; t++) {
            // 计算当前点坐标
            int16_t ft = t * 16 / seg_points; // Q4.4
            int16_t y = ((((a * ft) >> 4) + b) * ft) >> 4;
            y = ((y + c) * ft) >> 4;
            y = (y + d) >> 4;
            y = (y < 0) ? 0 : (y > 255) ? 255 : y;
            
            // 计算当前X坐标
            int16_t curr_x = x_start + seg*seg_points + t;
            
            // 使用Bresenham连接前后点
						OLED_WinDrawLine(&w_all, prev_x, prev_y, curr_x, y);
            // 更新前点坐标
            prev_x = curr_x;
            prev_y = y;
        }
    }
    
    // 强制绘制最后一个采样点
		OLED_WinDrawLine(&w_all, prev_x, prev_y, 118, input[9]);
}

//--------EQ设置页面相关函数
void OLED_EQPageEnterInit(PageAddr page_addr, uint16_t time) {
	
	EQPage *ep = (EQPage *)page_addr;
	ep->current_select = freq_63;
	for(eq_cfreq_e i=freq_63; i<=freq_16k; i++)
		ep->key_points[i] = calc_ycoor_from_boost(configures.preset_eq[configures.selected_preset][i]);
}

void OLED_EQPageShow(PageAddr page_addr, uint16_t time) {
	
	EQPage *ep = (EQPage *)page_addr;
	
	OLED_WinDrawHLine(&w_all, 0, 127, 54); // 底部横线
	OLED_WinDrawVLine(&w_all, 10, 52, 53); // 刻度线
	OLED_WinDrawVLine(&w_all, 22, 52, 53);
	OLED_WinDrawVLine(&w_all, 34, 52, 53);
	OLED_WinDrawVLine(&w_all, 46, 52, 53);
	OLED_WinDrawVLine(&w_all, 58, 52, 53);
	OLED_WinDrawVLine(&w_all, 70, 52, 53);
	OLED_WinDrawVLine(&w_all, 82, 52, 53);
	OLED_WinDrawVLine(&w_all, 94, 52, 53);
	OLED_WinDrawVLine(&w_all, 106, 52, 53);
	OLED_WinDrawVLine(&w_all, 118, 52, 53);
	switch (configures.selected_preset)
	{
		case 0:
			OLED_WinDrawStr(&w_all, 0, 57, Font_6_8, (uint8_t *)"Normal:");
			break;
		case 1:
			OLED_WinDrawStr(&w_all, 0, 57, Font_6_8, (uint8_t *)"Custom 1:");
			break;
		case 2:
			OLED_WinDrawStr(&w_all, 0, 57, Font_6_8, (uint8_t *)"Custom 2:");
			break;
		case 3:
			OLED_WinDrawStr(&w_all, 0, 57, Font_6_8, (uint8_t *)"Custom 3:");
			break;
	}
}


void OLED_EQPageReact(PageAddr page_addr, uint16_t time) {
	
		Page *p = (Page *)page_addr;
    EQPage *ep = (EQPage *)page_addr;
    String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead(); // 空时读出msg_none
		OLED_MsgQueClear(); 							// 这里暂时清空消息队列，可能会引发问题
	
		// 指示点的坐标
		uint8_t X = 0;
		uint8_t Y = calc_ycoor_from_boost(configures.preset_eq[configures.selected_preset][ep->current_select]);
		switch(ep->current_select)
		{
			case freq_63:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"63");
				X = FREQ_63_X_COOR;
				break;
			case freq_125:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"125");
				X = FREQ_125_X_COOR;
				break;
			case freq_250:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"250");
				X = FREQ_250_X_COOR;
				break;
			case freq_500:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"500");
				X = FREQ_500_X_COOR;
				break;
			case freq_1k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"1k");
				X = FREQ_1K_X_COOR;
				break;
			case freq_2k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"2k");
				X = FREQ_2K_X_COOR;
				break;
			case freq_4k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"4k");
				X = FREQ_4K_X_COOR;
				break;
			case freq_8k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"8k");
				X = FREQ_8K_X_COOR;
				break;
			case freq_12k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"12k");
				X = FREQ_12K_X_COOR;
				break;
			case freq_16k:
				OLED_WinDrawStr(&w_all, 55, 57, Font_6_8, (uint8_t *)"16k");
				X = FREQ_16K_X_COOR;
				break;
			default:
				break;
		}
		// 绘制指示点
		OLED_WinDrawHLine(&w_all, X-2, X+2, Y);
		OLED_WinDrawHLine(&w_all, X-1, X+1, Y+1);
		OLED_WinDrawHLine(&w_all, X-1, X+1, Y-1);
		OLED_WinDrawPoint(&w_all, X, Y+2);
		OLED_WinDrawPoint(&w_all, X, Y-2);
		
		// 绘制插值曲线
		ep->key_points[ep->current_select] = calc_ycoor_from_boost(configures.preset_eq[configures.selected_preset][ep->current_select]);
		OLED_DrawSmoothCurve(ep->key_points, true);
		
		// 显示boost值
		char numBuff[12];
		ui_itoa_str(configures.preset_eq[configures.selected_preset][ep->current_select], numBuff);
		OLED_WinDrawStr(&w_all, 108, 57, Font_6_8, (uint8_t *)numBuff);
		
		if(msg == msg_none){
		}
		else if(msg == msg_add){
			if(configures.preset_eq[configures.selected_preset][ep->current_select] < EQ_BOOST_MAX)
				configures.preset_eq[configures.selected_preset][ep->current_select] ++;
		}
		else if(msg == msg_sub){
			if(configures.preset_eq[configures.selected_preset][ep->current_select] > -EQ_BOOST_MAX)
				configures.preset_eq[configures.selected_preset][ep->current_select] --;
		}
		else if(msg == msg_up){    //prev
			if(ep->current_select > freq_63)
				ep->current_select --;
			else
				ep->current_select = freq_16k;
		}
		else if(msg == msg_down){  //next
			if(ep->current_select < freq_16k)
				ep->current_select ++;
			else
				ep->current_select = freq_63;
		}
		else if(msg == msg_click){ //play
		}
		else if(msg == msg_return){
			audio_save_configs_to_eeprom(&configures); // 存储音频配置
			p->cb(p, &(ep->option_array[10]));
		}
}



void OLED_EQPageInit(
    EQPage *eq_page,  			// 页面对象
    uint8_t item_num,       // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    Icon *icon_array,       // 整个页面的icon数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
{
		eq_page->page.page_type = type_userdef; //定义页面类型为"用户自定义"
    OLED_PageInit((PageAddr)eq_page, call_back);
    eq_page->page.init = OLED_EQPageEnterInit;
    eq_page->page.show = OLED_EQPageShow;
    eq_page->page.react = OLED_EQPageReact; // 关联处理函数(方法)
		eq_page->select_item = 0;
    eq_page->item_num = item_num;
		eq_page->option_array = option_array;
    eq_page->icon_array = icon_array;
    for (uint8_t i = 0; i < eq_page->item_num; i++)
        eq_page->option_array[i].order = i; // 选项序号标号
}
