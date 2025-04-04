/*
 * This is WouoUI User Defined Pages file.
 * 
 */
#include "WouoUI_udp.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"

#include "oledfont.h" 

extern const unsigned char icon_play_checked_16_16[];
extern const unsigned char icon_play_unchecked_16_16[];

extern const unsigned char icon_pause_checked_16_16[];
extern const unsigned char icon_pause_unchecked_16_16[];

extern const unsigned char icon_next_checked_16_16[];
extern const unsigned char icon_next_unchecked_16_16[];

extern const unsigned char icon_prev_checked_16_16[];
extern const unsigned char icon_prev_unchecked_16_16[];

//**********************每个以Page为基类的类都有对应的AnimInit、show、React方法

//--------Playing页面相关函数
void OLED_PlayingPageEnterInit(PageAddr page_addr, uint16_t time) {
//    p_cur_ui->tp_var.icon_x = 0;
//    p_cur_ui->tp_var.icon_x_tgt = TILE_ICON_S;
//    p_cur_ui->tp_var.icon_y = -TILE_ICON_H / 2;
//    p_cur_ui->tp_var.icon_y_tgt = TILE_ICON_U;
//    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
//    p_cur_ui->tp_var.bar_x_tgt = 0;
//    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
//    p_cur_ui->tp_var.title_y_tgt = OLED_HEIGHT - TILE_BAR_D - TILE_B_TITLE_FONT.Height;
}

void OLED_PlayingPageShow(PageAddr page_addr, uint16_t time) {
	
	OLED_WinDrawBMP(&w_all,24, 20, 16, 16, (uint8_t *)icon_prev_unchecked_16_16, 1);
	OLED_WinDrawBMP(&w_all,60, 20, 16, 16, (uint8_t *)icon_play_unchecked_16_16, 1);
	OLED_WinDrawBMP(&w_all,88, 20, 16, 16, (uint8_t *)icon_next_unchecked_16_16, 1);
}



void OLED_PlayingPageReact(PageAddr page_addr, uint16_t time) {
    TitlePage *tp = (TitlePage *)page_addr;
    String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead(); // 空时读出msg_none
	
	// 计数器与计数器标志，用以控制按下对应按键时屏幕Icon的变化，标志为1代表正在计数
		uint8_t counter[3] = {0};
		uint8_t cnt_switch[3] = {0};
		const uint8_t ICON_DISP_TIMEOUT = 100;
		
    switch (msg) {
    case msg_add: 
				break;
    case msg_sub:
        break;
    case msg_up:
				cnt_switch[0] = 1;
        break;
    case msg_down:
				cnt_switch[1] = 1;
        break;
    case msg_click:
				cnt_switch[2] = 1;
        break;
    case msg_return:
				
        break;
    case msg_home:
        break;
    default:
				if(cnt_switch[0]) {
					if(counter[0] < ICON_DISP_TIMEOUT)
							counter[0] ++;
					else{
							counter[0] = 0;
							cnt_switch[0] = 0;
					}
				}
				if(cnt_switch[1]) {
					if(counter[1] < ICON_DISP_TIMEOUT)
							counter[1] ++;
					else{
							counter[1] = 0;
							cnt_switch[1] = 0;
					}
				}
				if(cnt_switch[2]) {
					if(counter[2] < ICON_DISP_TIMEOUT)
							counter[2] ++;
					else{
							counter[2] = 0;
							cnt_switch[2] = 0;
					}
				}
        break;
    }
		
		if(cnt_switch[0]) 
			OLED_WinDrawBMP(&w_all,88, 20, 16, 16, (uint8_t *)icon_next_checked_16_16, 1);
		else
			OLED_WinDrawBMP(&w_all,88, 20, 16, 16, (uint8_t *)icon_next_unchecked_16_16, 1);
		if(cnt_switch[1]) 
			OLED_WinDrawBMP(&w_all,24, 20, 16, 16, (uint8_t *)icon_prev_checked_16_16, 1);
		else
			OLED_WinDrawBMP(&w_all,24, 20, 16, 16, (uint8_t *)icon_prev_unchecked_16_16, 1);
		if(cnt_switch[2]) 
			OLED_WinDrawBMP(&w_all,60, 20, 16, 16, (uint8_t *)icon_play_checked_16_16, 1);
		else
			OLED_WinDrawBMP(&w_all,60, 20, 16, 16, (uint8_t *)icon_play_unchecked_16_16, 1);
		
		
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
//    playing_page->option_array = option_array;
//    playing_page->icon_array = icon_array;
//    for (uint8_t i = 0; i < playing_page->item_num; i++)
//        playing_page->option_array[i].order = i; // 选项序号标号
}

