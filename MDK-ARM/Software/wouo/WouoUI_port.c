#include "WouoUI_port.h"

extern void simulateSendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]);

void WouoUI_port_Init(void) {}

void WouoUI_port_SendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]) {
    simulateSendBuff(buff);
}
