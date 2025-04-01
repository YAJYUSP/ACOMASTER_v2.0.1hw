#ifndef __OLED_CONF_H__
#define __OLED_CONF_H__

#include "stdbool.h"
#include "stdio.h"

// 使能软件动态刷新
#define SOFTWARE_DYNAMIC_REFRESH
// 使能硬件动态刷新
#define HARDWARE_DYNAMIC_REFRESH

#ifdef SOFTWARE_DYNAMIC_REFRESH
#define DYNAMIC_REFRESH_CNT 10
#endif

#define UI_INPUT_MSG_QUNEE_SIZE 4 // ui内部消息对列的大小(至少需要是2)

#define UI_CONWIN_ENABLE        1 // 是否使能 以"# " 为开头的选项使用确认弹窗
// #define PAGE_WAVE_ENABLE        1

//--------通用页面函数
#define UNUSED_PARAMETER(x)     ((void)x) // 对未使用参数的处理，防止警告

#endif
