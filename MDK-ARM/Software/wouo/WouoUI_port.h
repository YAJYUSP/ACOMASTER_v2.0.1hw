#ifndef __OLED_PORT_H__
#define __OLED_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"

#define OLED_LOG    (void)

#define OLED_WIDTH  128 // 屏幕宽
#define OLED_HEIGHT 64  // 屏幕高

// 对应要实现的接口函数
void WouoUI_port_Init(void);                                          // 初始化驱动
void WouoUI_port_SendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]); // 将8*128字节的buff一次性全部发送的函数

#ifdef __cplusplus
}
#endif

#endif
