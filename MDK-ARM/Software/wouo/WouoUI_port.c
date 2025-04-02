#include "WouoUI_port.h"
#include "WouoUI.h"

#include "oled.h"


extern void OLED_Refresh_FrameBuffer(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]);




/* 显示初始化函数，添加代码可以实现在初始化WouoUI的时候对屏幕进行初始化*/
void WouoUI_port_Init(void) {}

/* 帧刷新函数，WouoUI使用这个函数来对屏幕进行全局刷新*/
void WouoUI_port_SendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]) {
    OLED_Refresh_FrameBuffer(buff);
}
