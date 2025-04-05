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
ListPage about_page;
RadioPage about_origin_page;
RadioPage about_wououi_page;
RadioPage about_version_page;


//--------定义每个页面需要的一些参数

// 页面内选项个数
#define PLAY_PAGE_NUM         5
#define MAIN_PAGE_NUM         3
#define SETTING_PAGE_NUM      13
#define ABOUT_PAGE_NUM        3
#define ABOUT_ORIGIN_PAGE_NUM 6
#define ABOUT_WOUOUI_PAGE_NUM 9
#define ABOUT_PAGEVERSION_NUM 8

// main页面的选项
const Option mian_option_array[MAIN_PAGE_NUM] =
    {
        {.text = (char *)"+ Playing"},
        {.text = (char *)"+ Settings"},
        {.text = (char *)"! Info", .content = (char *)"    WARNING\nThis is a\ninfo test!"},
        // {.text = (char *)"# Conf", .content = (char *)"    WARNING\nThis is a\nconf test!"},
        // {.text = (char *)"% Spin", .val = 123456, .min = -500000, .max = 500000, .decimalNum = DecimalNum_2},
};
// main页面的图标
const Icon main_icon_array[MAIN_PAGE_NUM] =
    {
        [0] = {0xFC, 0xFE, 0xFF, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
               0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0x07, 0x0F, 0x1F, 0x3F, 0xFF, 0xFE, 0xFC, 0xFF, 0x01,
               0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC,
               0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xF0, 0xC0, 0x00,
               0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3C, 0x3C, 0x3C, 0x1E, 0x1F, 0x0F, 0x03, 0x00, 0x00,
               0x1F, 0x3F, 0x3F, 0x1F, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFC,
               0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0,
               0xF8, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xDF, 0xCF}, // logo
        [1] = {0xFC, 0xFE, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x83, 0x81, 0x01, 0x01, 0x81, 0xE1, 0xE1, 0xE1,
               0xE1, 0x81, 0x01, 0x81, 0x81, 0x83, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFE, 0xFC, 0xFF, 0x01,
               0x00, 0x00, 0x00, 0xE0, 0xE0, 0xF3, 0xFF, 0xFF, 0x3F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07,
               0x0F, 0x3F, 0xFF, 0xFF, 0xF7, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xE0, 0x80, 0x00,
               0x00, 0x01, 0x01, 0x3B, 0x7F, 0x7F, 0x7F, 0x3C, 0x78, 0xF8, 0xF0, 0xF0, 0xF8, 0x78, 0x3C, 0x3F,
               0x7F, 0x7F, 0x33, 0x01, 0x01, 0x00, 0x00, 0x80, 0xE0, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFC,
               0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE1, 0xE1, 0xE1, 0xE1, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0,
               0xF0, 0xF8, 0xFC, 0xFC, 0xFF, 0xFF, 0xDF, 0xCF}, // Setting
        [2] = {0xFC, 0xFE, 0x07, 0xF3, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0x03, 0x03,
               0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xF3, 0x07, 0xFE, 0xFC, 0xFF, 0xFF,
               0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xBF, 0x3F, 0x7F, 0x20, 0x08, 0x8F, 0x07,
               0x61, 0x30, 0x36, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xE7,
               0xE7, 0xE7, 0xE7, 0xE7, 0xC7, 0xDB, 0xC3, 0xE1, 0xF8, 0xFC, 0x04, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xCF, 0xDF, 0xF8, 0xF3, 0xF7, 0xF7,
               0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7,
               0xF7, 0xF7, 0xF7, 0xF7, 0xF3, 0xF8, 0xDF, 0xCF}, // Curve
        // [3] = {0xFC, 0xFE, 0x1F, 0x07, 0x07, 0xC3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0x63, 0x63, 0x63,
        //        0x63, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xC3, 0x07, 0x07, 0x1F, 0xFE, 0xFC, 0xFF, 0xFF,
        //        0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x01, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00,
        //        0x01, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
        //        0x00, 0xFF, 0xFF, 0xFF, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        //        0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xCF, 0xDF, 0xFE, 0xF8, 0xF8, 0xF0,
        //        0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1,
        //        0xF1, 0xF1, 0xF0, 0xF8, 0xF8, 0xFE, 0xDF, 0xCF}, // DigitalLock
        // [4] = {0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07,
        //        0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFF, 0xFF,
        //        0xFF, 0x3F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xEE, 0xE6, 0x66, 0x00,
        //        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        //        0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1F, 0x1F, 0x08, 0x00, 0x00, 0x00,
        //        0x00, 0x00, 0x00, 0x00, 0x60, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
        //        0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
        //        0xF8, 0xF8, 0xF8, 0xF8, 0xFF, 0xFF, 0xDF, 0xCF} // about
};

// Play页面的选项字符串表，不用于显示，用于回调函数传参
const Option play_option_array[ABOUT_PAGEVERSION_NUM] =
{
        {.text = (char *)"prev"},
        {.text = (char *)"next"},
        {.text = (char *)"play"},
        {.text = (char *)"volp"},
        {.text = (char *)"vols"},
				{.text = (char *)"return"},
};


// 设置页面的列表选项数组
const Option setting_option_array[SETTING_PAGE_NUM] =
    {
        {.text = (char *)"- List"}, // 第一个做说明标签，没有功能
        {.text = (char *)"% spin box 1", .val = 123456, .min = -500000, .max = 500000, .decimalNum = DecimalNum_1},
        {.text = (char *)"% spin box 2", .val = 0, .min = 0, .max = 9999, .decimalNum = DecimalNum_0},
        {.text = (char *)"~ slide val 1", .val = 10, .step = 10, .min = -100, .max = 149},
        {.text = (char *)"~ slide val 2", .val = 20, .step = 5, .min = 0, .max = 100},
        {.text = (char *)"= fixed val 1", .val = 123},
        {.text = (char *)"= fixed val 2", .val = 4567, .decimalNum = DecimalNum_2},
        {.text = (char *)"! Info test 1", .content = (char *)"This is a\ninfo win test!"},
        {.text = (char *)"! Info test 2", .content = (char *)"    INFO\nThis is a\ninfo win test!"},
        {.text = (char *)"# conf test 1", .content = (char *)"Sure to enable\n/disable the\nfunction?", .step = 1},
        {.text = (char *)"# conf test 2", .content = (char *)"    WARNING\nSure to enable\n/disable the\nfunction?", .step = 1},
        {.text = (char *)"@ Option box 1", .step = 1},
        {.text = (char *)"@ Option box 2", .step = 1},
};

// about页面的选项数组
const Option about_option_array[ABOUT_PAGE_NUM] =
    {
        {.text = (char *)"+ Radio box"},    // 原wououi的说明
        {.text = (char *)"+ About wouo"},   // 第一项只是说明
        {.text = (char *)"+ Page version"}, // Page version的说明
};

// about页面->Radio box页面的数组
const Option about_origin_array[ABOUT_ORIGIN_PAGE_NUM] =
    {
        {.text = (char *)"- Radio box"},
        {.text = (char *)"# test1", .val = 1, .step = 1},
        {.text = (char *)"# test2", .val = 0, .step = 1},
        {.text = (char *)"@ test3", .step = 1},
        {.text = (char *)"@ test4", .step = 1},
        {.text = (char *)"@ test5", .step = 1},
};

// about页面->About wouo页面的数组
const Option about_wououi_array[ABOUT_WOUOUI_PAGE_NUM] =
    {
        {.text = (char *)"- From WouoUI1.2"},
        {.text = (char *)"- Thanks for the "},
        {.text = (char *)"  original author"},
        {.text = (char *)"! Bilibili UID:"},
        {.text = (char *)"@ 9182439"},
        {.text = (char *)"- Github URL: "},
        {.text = (char *)"@ https://"},
        {.text = (char *)"  github.com/"},
        {.text = (char *)"  RQNG/WouoUI"},
};

// about页面->Page version页面的数组
const Option about_version_array[ABOUT_PAGEVERSION_NUM] =
    {
        {.text = (char *)"- Page Version"},
        {.text = (char *)"- MyBilili UID:"},
        {.text = (char *)"@ 679703519"},
        {.text = (char *)"- Github URL:"},
        {.text = (char *)"@ https://"},
        {.text = (char *)"  github.com/"},
        {.text = (char *)"  Sheep118/WouoUI"},
        {.text = (char *)"  -PageVersion"},
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
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_page);
    }
}

// setting页面的回调函数，主要用于参数赋值
void SettingPage_CallBack(const Page *cur_page_addr, Option *select_item) {
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

// setting页面的回调函数，主要用于页面跳转
void About_CallBack(const Page *cur_page_addr, Option *select_item) {
    switch (select_item->order) { // 第0项是说明文字
    case 0:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_origin_page);
        break;
    case 1:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_wououi_page);
        break;
    case 2:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_version_page);
        break;
    default:
        break;
    }
//			if(select_item->order == 0)
//				OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_origin_page);
//			else if(select_item->order == 1)
//				OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_wououi_page);
//			else if(select_item->order == 2)
//				OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_version_page);
	
	
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
    OLED_ListPageInit(&about_page, ABOUT_PAGE_NUM, (Option *)about_option_array, Setting_none, About_CallBack);

    OLED_ListPageInit(&about_origin_page, ABOUT_ORIGIN_PAGE_NUM, (Option *)about_origin_array, Setting_radio, NULL);
    OLED_ListPageInit(&about_wououi_page, ABOUT_WOUOUI_PAGE_NUM, (Option *)about_wououi_array, Setting_none, NULL);
    OLED_ListPageInit(&about_version_page, ABOUT_PAGEVERSION_NUM, (Option *)about_version_array, Setting_none, NULL);
}
