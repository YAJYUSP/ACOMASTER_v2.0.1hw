/*
 * This is WouoUI User Defined Pages file.
 * 
 */
#include "WouoUI_udp.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"

#include "oledfont.h" 
#include "main.h"

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

}

void OLED_PlayingPageShow(PageAddr page_addr, uint16_t time) {
	
//	OLED_WinDrawBMP(&w_all,24, 20, 16, 16, (uint8_t *)icon_prev_unchecked_16_16, 1);
//	OLED_WinDrawBMP(&w_all,60, 20, 16, 16, (uint8_t *)icon_play_unchecked_16_16, 1);
//	OLED_WinDrawBMP(&w_all,88, 20, 16, 16, (uint8_t *)icon_next_unchecked_16_16, 1);

		OLED_WinDrawStr(&w_all, 88, 20, Font_12_24, (unsigned char *)"0");
		OLED_WinDrawStr(&w_all, 24, 20, Font_12_24, (unsigned char *)"0");
		OLED_WinDrawStr(&w_all, 60, 20, Font_12_24, (unsigned char *)"0");
}



InputMsg _msg = msg_none;

void OLED_PlayingPageReact(PageAddr page_addr, uint16_t time) {
    PlayingPage *pp = (PlayingPage *)page_addr;
    String selcet_string = NULL;
    _msg = OLED_MsgQueRead(); // 空时读出msg_none
		OLED_MsgQueClear(); // 这里暂时清空消息队列，可能会引发问题------------------======================================================
	
		// 计数器与计数器标志，用以控制按下对应按键时屏幕Icon的变化，标志为1代表正在计数
		static uint8_t counter[3] = {0};
		static uint8_t cnt_switch[3] = {0};
		uint8_t ICON_DISP_TIMEOUT = 50; // 实际时间 = ICON_DISP_TIMEOUT * time
		
		if(_msg == msg_none){
			if(cnt_switch[0]) {
					if(counter[0] < ICON_DISP_TIMEOUT)
							counter[0] ++;
					else{
							counter[0] = 0;
							cnt_switch[0] = 0;
						
							OLED_WinDrawStr(&w_all, 88, 20, Font_12_24, (unsigned char *)" ");
							OLED_WinDrawStr(&w_all, 88, 20, Font_12_24, (unsigned char *)"0");
					}
				}
				if(cnt_switch[1]) {
					if(counter[1] < ICON_DISP_TIMEOUT)
							counter[1] ++;
					else{
							counter[1] = 0;
							cnt_switch[1] = 0;
							OLED_WinDrawStr(&w_all, 24, 20, Font_12_24, (unsigned char *)" ");
							OLED_WinDrawStr(&w_all, 24, 20, Font_12_24, (unsigned char *)"0");
					}
				}
				if(cnt_switch[2]) {
					if(counter[2] < ICON_DISP_TIMEOUT)
							counter[2] ++;
					else{
							counter[2] = 0;
							cnt_switch[2] = 0;
							OLED_WinDrawStr(&w_all, 60, 20, Font_12_24, (unsigned char *)" ");
							OLED_WinDrawStr(&w_all, 60, 20, Font_12_24, (unsigned char *)"0");
					}
				}
		}
		else if(_msg == msg_add){
			
		}
		else if(_msg == msg_sub){
			
		}
		else if(_msg == msg_up){
			cnt_switch[0] = 1;
			OLED_WinDrawStr(&w_all, 88, 20, Font_12_24, (unsigned char *)" ");
			OLED_WinDrawStr(&w_all, 88, 20, Font_12_24, (unsigned char *)"1");
		}
		else if(_msg == msg_down){
			cnt_switch[1] = 1;
			OLED_WinDrawStr(&w_all, 24, 20, Font_12_24, (unsigned char *)" ");
			OLED_WinDrawStr(&w_all, 24, 20, Font_12_24, (unsigned char *)"1");
		}
		else if(_msg == msg_click){
			cnt_switch[2] = 1;
			OLED_WinDrawStr(&w_all, 60, 20, Font_12_24, (unsigned char *)" ");
			OLED_WinDrawStr(&w_all, 60, 20, Font_12_24, (unsigned char *)"1");
		}
		else if(_msg == msg_return){
			
		}
		
		
		
//    switch (_msg) {
//		case msg_none: 
//				if(cnt_switch[0]) {
//					if(counter[0] < ICON_DISP_TIMEOUT)
//							counter[0] ++;
//					else{
//							counter[0] = 0;
//							cnt_switch[0] = 0;
//					}
//				}
//				if(cnt_switch[1]) {
//					if(counter[1] < ICON_DISP_TIMEOUT)
//							counter[1] ++;
//					else{
//							counter[1] = 0;
//							cnt_switch[1] = 0;
//					}
//				}
//				if(cnt_switch[2]) {
//					if(counter[2] < ICON_DISP_TIMEOUT)
//							counter[2] ++;
//					else{
//							counter[2] = 0;
//							cnt_switch[2] = 0;
//					}
//				}
//				break;
//    case msg_add: 
//				break;
//    case msg_sub:
//        break;
//    case msg_up:
//        break;
//    case msg_down:
//        break;
//    case msg_click:
//        break;
//    case msg_return:
//        break;
//    case msg_home:
//        break;
//    default:
//        break;
//    }
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
}

