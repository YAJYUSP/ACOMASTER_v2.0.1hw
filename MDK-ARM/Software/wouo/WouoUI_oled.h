#ifndef __OLED_G_H__
#define __OLED_G_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI_font.h"
#include "WouoUI_port.h"
#include "math.h"
#include "string.h"

#define OLED_WIDTH        128 // 屏幕宽
#define OLED_HEIGHT       64  // 屏幕高

#define OLED_MIDDLE_H     OLED_WIDTH / 2  // 屏幕水平中间值
#define OLED_MIDDLE_V     OLED_HEIGHT / 2 // 屏幕垂直中间值

#define OLED_LINE_SPACING 1 // 绘制字符串时行间距

extern uint8_t oled_buff[OLED_HEIGHT / 8][OLED_WIDTH];         // 全局buff，对应oled中的GRAM
extern uint8_t oled_buff_dynamic[OLED_HEIGHT / 8][OLED_WIDTH]; // 适用于硬件动态刷新的GRAM

typedef struct
{
    int16_t start_x;
    int16_t start_y;
    int16_t w;
    int16_t h;
} window;

typedef enum {
    BLUR_0_4 = 0x00,
    BLUR_1_4,
    BLUR_2_4,
    BLUR_3_4,
    BLUR_4_4,
} BLUR_DEGREE;

typedef enum {
    RD_LEFT_UP = 0x00, // 射线从左上角出发
    RD_LEFT_DOWN,      // 射线从左下角出发
    RD_RIGHT_UP,       // 射线从右上角出发
    RD_RIGHT_DOWN,     // 射线从右下角出发
    RD_RIGHT,          // 射线从水平向右
    RD_LEFT,           // 射线从水平向左
} RaderDirection;

typedef enum {
    COLOR_NORMAL = 0,
    COLOR_XOR
} OLED_COLOR_MODE;

typedef enum {
    OLED_BLACK = 0,
    OLED_WHITE
} OLED_COLOR;

void OLED_ClearBuff(void);
void OLED_SetPointColor(uint8_t color);
void OLED_SendBuff(void);
void OLED_SendBuffDynamic(void);

int16_t OLED_WinDrawASCII(window *win, int16_t x, int16_t y, sFONT font, char c);
void OLED_WinDrawStr(window *win, int16_t x, int16_t y, sFONT font, uint8_t *str);
uint16_t OLED_GetStrWidth(const char *str, sFONT font);
uint16_t OLED_GetStrHeight(const char *str, sFONT font);
void OLED_WinDrawVLine(window *win, int16_t x, int16_t y_start, int16_t y_end);
void OLED_WinDrawHLine(window *win, int16_t x_start, int16_t x_end, int16_t y);
void OLED_WinDrawRBox(window *win, int16_t x_start, int16_t y_start, int16_t width, int16_t height, uint8_t r);
void OLED_WinDrawRBoxEmpty(window *win, int16_t x_start, int16_t y_start, int16_t width, int16_t height, uint8_t r);
void OLED_WinDrawBoxRightAngle(window *win, int16_t x_start, int16_t y_start, int16_t width, int16_t height, uint8_t r);
void OLED_WinDrawBMP(window *win, int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *BMP, uint8_t color);
void OLED_WinDrawPoint(window *win, int16_t x, int16_t y);
void OLED_WinDrawLine(window *win, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void OLED_AllBlur(BLUR_DEGREE blur);
uint8_t OLED_WinDrawRaderPic(window *win, const uint8_t *pic, int16_t start_x, uint16_t start_y, uint8_t width, uint8_t height, uint16_t *p_end_point,
                             RaderDirection direct, uint8_t enable_all_point_scan, uint8_t scan_rate);

#ifdef __cplusplus
}
#endif

#endif
