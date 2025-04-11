#include "WouoUI_user.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"
#include "WouoUI_udp.h"

#include "main.h"
#include "audio.h"
#include "qcc5125.h"


//--------定义页面对象
PlayingPage playing_page;
TitlePage main_page;

ListPage setting_page;
ListPage setting_eqpreset_page;
EQPage setting_eq_page;


//--------定义每个页面需要的一些参数

// 页面内选项个数
#define PLAY_PAGE_NUM         6
#define MAIN_PAGE_NUM         3
 
#define SETTING_PAGE_NUM      7
#define SETTING_EQPRESET_NUM  5 
#define SETTING_EQ_NUM        11 


// main页面的图标,30x30
const Icon main_icon_array[MAIN_PAGE_NUM] =
{
				[0] = {0xF0,0xF8,0xFC,0x7E,0x3F,0x1F,0x0F,0x07,0x07,0x83,0x83,0x01,0x01,0x01,0x01,0x01,
							 0x01,0x01,0x01,0x03,0x03,0x07,0x07,0x0F,0x1F,0x3F,0x7E,0xFC,0xF8,0xF0,0xFF,0x07,
							 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFC,0xFC,0xF8,
							 0xF8,0xF0,0xF0,0xE0,0xE0,0xC0,0xC0,0x00,0x00,0x01,0x07,0xFF,0xFF,0xF8,0xE0,0x80,
							 0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x3F,0x3F,0x1F,0x1F,0x0F,0x0F,0x07,0x07,0x03,
							 0x03,0x01,0x01,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFF,0x03,0x07,0x0F,0x1F,0x3F,0x3E,
							 0x3C,0x38,0x38,0x30,0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x30,0x38,
							 0x38,0x3C,0x3E,0x3F,0x1F,0x0F,0x07,0x03}, // Playing
        [1] = {0xFC, 0xFE, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x83, 0x81, 0x01, 0x01, 0x81, 0xE1, 0xE1, 0xE1,
               0xE1, 0x81, 0x01, 0x81, 0x81, 0x83, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFE, 0xFC, 0xFF, 0x01,
               0x00, 0x00, 0x00, 0xE0, 0xE0, 0xF3, 0xFF, 0xFF, 0x3F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07,
               0x0F, 0x3F, 0xFF, 0xFF, 0xF7, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xE0, 0x80, 0x00,
               0x00, 0x01, 0x01, 0x3B, 0x7F, 0x7F, 0x7F, 0x3C, 0x78, 0xF8, 0xF0, 0xF0, 0xF8, 0x78, 0x3C, 0x3F,
               0x7F, 0x7F, 0x33, 0x01, 0x01, 0x00, 0x00, 0x80, 0xE0, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFC,
               0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE1, 0xE1, 0xE1, 0xE1, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0,
               0xF0, 0xF8, 0xFC, 0xFC, 0xFF, 0xFF, 0xDF, 0xCF}, // Setting
        [2] = {0xF0,0xF8,0xFC,0x7E,0x3F,0x1F,0x0F,0x07,0x07,0x03,0x03,0x01,0x01,0xF1,0xF1,0xF1,
							 0xF1,0x01,0x01,0x03,0x03,0x07,0x07,0x0F,0x1F,0x3F,0x7E,0xFC,0xF8,0xF0,0xFF,0x07,
							 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0xFC,0xFE,0xFE,0xFE,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0xFF,0xFF,0xF8,0xE0,0x80,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xFF,0xFF,0xFF,0xFF,0x80,0x80,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,0xFF,0x03,0x07,0x0F,0x1F,0x3F,0x3E,
							 0x3C,0x38,0x38,0x30,0x30,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x30,0x30,0x38,
							 0x38,0x3C,0x3E,0x3F,0x1F,0x0F,0x07,0x03}, // Info
};


// main页面的选项字符串表
const Option mian_option_array[MAIN_PAGE_NUM] =
{
        {.text = (char *)"+ Playing"},
        {.text = (char *)"+ Settings"},
        {.text = (char *)"! Info", .content = (char *)"   ACOMASTER\nCopyright 2022\n@ Jesse Jia"},
};
// Play页面的选项字符串表，不用于显示，用于回调函数传参
const Option play_option_array[PLAY_PAGE_NUM] =
{
        {.text = (char *)"prev"},
        {.text = (char *)"next"},
        {.text = (char *)"play"},
        {.text = (char *)"volp"},
        {.text = (char *)"vols"},
				{.text = (char *)"return"},
};

// setting页面的选项字符串表
Option setting_option_array[SETTING_PAGE_NUM] =
{
        {.text = (char *)"- Settings -"},   // 选择EQ预设
				{.text = (char *)"+ EQ Presets"},   // 进入选择EQ预设
        {.text = (char *)"+ Edit EQ Preset"},   // 进入选择EQ预设
        {.text = (char *)"# HP Detecting", .val = 0, .step = 1},  //预留radio box
				{.text = (char *)"# DSEE HX", .val = 0, .step = 1},  //预留radio box
				{.text = (char *)"# Auto PWR-OFF", .val = 0, .step = 1},  //预留radio box
				{.text = (char *)"# PWR-OFF Timer", .val = 0, .step = 1},  //预留radio box
};  
// setting->EQ Preset页面的选项字符串表
Option setting_preset_option_array[SETTING_EQPRESET_NUM] =
{
        {.text = (char *)"- EQ Presets -"},
        {.text = (char *)"# Normal", .val = 1, .step = 1},
        {.text = (char *)"# Custom 1", .val = 0, .step = 1},
        {.text = (char *)"# Custom 2", .val = 0, .step = 1},
				{.text = (char *)"# Custom 3", .val = 0, .step = 1},
};
// setting->EQ设置页面的选项字符串表
Option setting_eq_option_array[SETTING_EQ_NUM] =
{
        {.text = (char *)"freq_63"},
				{.text = (char *)"freq_125"},
				{.text = (char *)"freq_250"},
				{.text = (char *)"freq_500"},
				{.text = (char *)"freq_1k"},
				{.text = (char *)"freq_2k"},
				{.text = (char *)"freq_4k"},
				{.text = (char *)"freq_8k"},
				{.text = (char *)"freq_12k"},
				{.text = (char *)"freq_16k"},
				{.text = (char *)"return"},
};


//--------定义每个页面的回调函数

// play页面的回调函数
void PlayingPage_CallBack(const Page *cur_page_addr, Option *select_item) {

	if (!strcmp(select_item->text, "prev")) {
		qcc5125_btn_press(KEY_PREV, 200);
	} 
	if (!strcmp(select_item->text,"next")) {
		qcc5125_btn_press(KEY_NEXT, 200);
	} 
	if (!strcmp(select_item->text, "play")) {
		qcc5125_btn_press(KEY_PLAY, 200);
	} 
	if (!strcmp(select_item->text, "volp")) {
		qcc5125_btn_press(KEY_NEXT, 550);
	}
	if (!strcmp(select_item->text, "vols")) {
		qcc5125_btn_press(KEY_PREV, 550);
	}
	if (!strcmp(select_item->text, "return")) {
		OLED_UIJumpToPage((PageAddr)cur_page_addr, &main_page);
	}
}

// main页面的回调函数，主要用于页面跳转
void MainPage_CallBack(const Page *cur_page_addr, Option *select_item) {
    if (!strcmp(select_item->text, "+ Playing")) {
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &playing_page);
    } else if (!strcmp(select_item->text, "+ Settings")) {
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_page);
    }
}
// setting页面的回调函数
void SettingPage_CallBack(const Page *cur_page_addr, Option *select_item) {
 
//	if (!strcmp(select_item->text, "+ EQ Presets")) {
//		OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_eqpreset_page);
//	} else if (!strcmp(select_item->text, "+ Edit EQ Preset")) {
//        OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_eq_page );
//	}
	switch (select_item->order) { 
	case 0:
			break;
	case 1:
			OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_eqpreset_page);
			break;
	case 2:
			OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_eq_page);
			break;
	default:
			break;
	}
}

extern audio_config_t configures;
// setting->EQPreset页面(继承自ListPage)的回调函数
void Setting_EQPresetPage_CallBack(const Page *cur_page_addr, Option *select_item) {
	if(select_item->order != 0)
		configures.selected_preset = select_item->order - 1;
}
// setting->EQ页面的回调函数
void Setting_EQPage_CallBack(const Page *cur_page_addr, Option *select_item) {
	if (!strcmp(select_item->text, "return")) {
		OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_page);
	}
}

//--------------页面初始化函数，供主函数调用


void TestUI_Init(void) {
    OLED_ClearBuff();      // 清空缓存
    OLED_SendBuff();       // 刷新屏幕(清空屏幕)
    OLED_SetPointColor(1); // 设置绘制颜色

    // 设置界面选项
	 
		OLED_PlayingPageInit(&playing_page, PLAY_PAGE_NUM, (Option *)play_option_array, NULL, PlayingPage_CallBack);
	
    OLED_TitlePageInit(&main_page, MAIN_PAGE_NUM, (Option *)mian_option_array, main_icon_array, MainPage_CallBack);

    OLED_ListPageInit(&setting_page, SETTING_PAGE_NUM, (Option *)setting_option_array, Setting_none, SettingPage_CallBack);
		OLED_ListPageInit(&setting_eqpreset_page, SETTING_EQPRESET_NUM, (Option *)setting_preset_option_array, Setting_radio, Setting_EQPresetPage_CallBack);
		OLED_EQPageInit(&setting_eq_page, SETTING_EQ_NUM, (Option *)setting_eq_option_array, NULL, Setting_EQPage_CallBack);
}

