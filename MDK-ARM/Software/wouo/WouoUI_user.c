#include "WouoUI_user.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"
#include "WouoUI_udp.h"

#include "main.h"
#include "qcc5125.h"


//--------定义页面对象
PlayingPage playing_page;
TitlePage main_page;

ListPage setting_page;
ListPage setting_eqpreset_page;

//--------定义每个页面需要的一些参数

// 页面内选项个数
#define PLAY_PAGE_NUM         6
#define MAIN_PAGE_NUM         3
 
#define SETTING_PAGE_NUM      7
#define SETTING_EQPRESET_NUM  5 


// main页面的选项
const Option mian_option_array[MAIN_PAGE_NUM] =
{
        {.text = (char *)"+ Playing"},
        {.text = (char *)"+ Settings"},
        {.text = (char *)"! Info", .content = (char *)"   ACOMASTER\nCopyright 2022\n@ Jesse Jia"},
        // {.text = (char *)"# Conf", .content = (char *)"    WARNING\nThis is a\nconf test!"},
        // {.text = (char *)"% Spin", .val = 123456, .min = -500000, .max = 500000, .decimalNum = DecimalNum_2},
};
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



//--------定义每个页面的回调函数

// play页面的回调函数,暂时没有用到
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

// setting页面的回调函数，主要用于参数赋值
void SettingPage_CallBack(const Page *cur_page_addr, Option *select_item) {
 
	if (!strcmp(select_item->text, "+ EQ Presets")) {
		OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_eqpreset_page);
	} 
	
	
	// switch (select_item->order)
    // {                           // 由于第0项是说明文字“Setting”
    // case 1:
    //     g_default_ui_para.ani_param[TILE_ANI] = select_item->val;
    //     break; // ani_tile
    // case 2:
    //     g_default_ui_para.ani_param[LIST_ANI] = select_item->val;
    //     break; // ani_list
    // case 3:
    //     g_default_ui_para.ufd_param[TILE_UFD] = select_item->val;
    //     break; // ani_tile
    // case 4:
    //     g_default_ui_para.ufd_param[LIST_UFD] = select_item->val;
    //     break; // ani_list
    // case 5:
    //     g_default_ui_para.loop_param[TILE_UFD] = select_item->val;
    //     break; // loop_tile
    // case 6:
    //     g_default_ui_para.loop_param[LIST_UFD] = select_item->val;
    //     break; // loop_list
    // case 7:
    //     g_default_ui_para.valwin_broken = select_item->val;
    //     break; // ValWin Broken
    // case 8:
    //     g_default_ui_para.conwin_broken = select_item->val;
    //     break; // ConWin Broken
    // case 9:
    //     g_default_ui_para.digital_ripple = select_item->val;
    //     break; // Digital Ripple Enable/not
    // case 10:
    //     g_default_ui_para.raderpic_scan_mode = select_item->val;
    //     break; // RaderPic scan mode
    // case 11:
    //     g_default_ui_para.raderpic_scan_rate = select_item->val;
    //     break; // RaderPic scan rate
    // case 12:
    //     g_default_ui_para.raderpic_move_rate = select_item->val;
    //     break; // RaderPic move rate
    // default:
    //     break;
    // }
}

// setting->EQPreset页面的回调函数，主要用于参数赋值
void Setting_EQPresetPage_CallBack(const Page *cur_page_addr, Option *select_item) {
	
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

}

