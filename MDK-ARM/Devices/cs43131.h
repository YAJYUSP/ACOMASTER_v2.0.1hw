#ifndef   __CS43131_H
#define  __CS43131_H

// 0110000 if ADR is grounded
#define CS43131_ADDR       0x30
#define CS43131_WRITE_ADDR 0x60
#define CS43131_READ_ADDR  0x61

#include "stdint.h"


void bsp_43131_init(void);
void bsp_43131_set_volume(uint8_t vol);

static void CS43131_WriteReg(uint32_t reg, uint8_t val);
static uint8_t CS43131_ReadReg(uint32_t reg);
static uint8_t CS_I2C_ReadPayload(uint8_t addr, uint8_t *tx_payload, uint8_t tx_len, uint8_t *rx_payload, uint8_t rx_len);
static uint8_t CS_I2C_WritePayload(uint8_t addr, uint8_t *payload, uint8_t len) ;


#endif
















