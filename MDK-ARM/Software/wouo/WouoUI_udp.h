#ifndef __WOUOUI_UDP_H__
#define __WOUOUI_UDP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI.h"
#include "adau1761.h"


typedef struct PlayingPage { 	// 播放器图标页面
    Page page;             	 	// 基础页面信息
    PageSetting page_setting; // 页面设置
		uint8_t item_num;      // 页面选项个数，option_array和icon_array个数需与此一致
    Option *option_array;  // 选项类型的数组(由于数组大小不确定，使用指针代替)
    Icon *icon_array;      // 图标数组(由于数组大小不确定，使用指针代替)
    uint8_t select_item;   // 选中选项
} PlayingPage;               	// 播放器页面类型(所有类型页面，类型成员为第一个，方便查看)

typedef struct EQPage { 	 // EQ设置页面
    Page page;             	 	// 基础页面信息
    PageSetting page_setting; // 页面设置
		uint8_t item_num;      // 页面选项个数，option_array和icon_array个数需与此一致
    Option *option_array;  // 选项类型的数组(由于数组大小不确定，使用指针代替)
    Icon *icon_array;      // 图标数组(由于数组大小不确定，使用指针代替)
    uint8_t select_item;   // 选中的中心频率
	
		eq_cfreq_e current_select; // 当前选中的eq段
		int8_t current_eqboost; // 当前选中的eq段对应的boost值
	           
	
	
	
} EQPage;               	 // EQ设置页面(所有类型页面，类型成员为第一个，方便查看)
	
//-------Playing页面相关函数
void OLED_PlayingPageEnterInit(PageAddr page_addr, uint16_t time);
void OLED_PlayingPageShow(PageAddr page_addr, uint16_t time);
void OLED_PlayingPageReact(PageAddr page_addr, uint16_t time);
void OLED_PlayingPageInit(PlayingPage *title_page, uint8_t item_num, Option *option_array, Icon *icon_array, CallBackFunc call_back);
//-------EQ页面相关函数
void OLED_EQPageEnterInit(PageAddr page_addr, uint16_t time);
void OLED_EQPageShow(PageAddr page_addr, uint16_t time);
void OLED_EQPageReact(PageAddr page_addr, uint16_t time);
void OLED_EQPageInit(EQPage *eq_page, uint8_t item_num, Option *option_array, Icon *icon_array, CallBackFunc call_back);
	
	
	
	
	
	
	
	
	
	
	
	
	
void TestUI_Init(void);

#ifdef __cplusplus
}
#endif

#endif
