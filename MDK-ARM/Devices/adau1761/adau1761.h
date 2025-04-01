#ifndef _ADAU1761_H_
#define _ADAU1761_H_

#include "adau1761_REG.h"

#define DEVICE_ARCHITECTURE_IC_2                  "ADAU176x"
#define DEVICE_ADDR_IC_2                          0x70

#define EQ_BOOST_MAX  15

typedef unsigned char ADI_REG_TYPE;

typedef struct
{
		//boost of filter
		float eq_boost;
		//eq center frequency
		uint16_t center_freq;
		//(Q of filter; must be greater than or equal to 0.01
		float kQ;
		//linear gain applied to the signal
		float gain;
		//sample rate of project
		uint32_t prj_Fs;
		float gain_linear;

		float otpt_B0;
		float otpt_B1;
		float otpt_B2;
		float otpt_A1;
		float otpt_A2;
	
		//safeload register start addr
		uint16_t safeload_addr;
}dsp_eq_t;

//每段eq的boost设定值
typedef struct
{
		float setting_63;
		float setting_125;
		float setting_250;
		float setting_500;
		float setting_1000;
		float setting_2000;
		float setting_4000;
		float setting_8000;
		float setting_12000;
		float setting_16000;
}dsp_eq_setting_t;


extern int8_t eq_setting_boost[10];
extern int8_t eq_setting_boost_last[10];

//10 eq channels
typedef enum
{
		eq_63, eq_125, eq_250, eq_500, eq_1000,
		eq_2000, eq_4000, eq_8000, eq_12000, eq_16000
}eq_chnl_e;



extern dsp_eq_t DSP_EQ[10];

int point_to_int32(float point);

void default_download_IC_2(void);
void bsp_adau1761_eq_init(void);
void bsp_adau1761_eq_set(eq_chnl_e eq_channel, float boost);
void bsp_adau1761_eq_data_conv(dsp_eq_t *EQ);
void bsp_adau1761_eq_data_conv_all(void);
							 
void bsp_adau1761_eq_download(eq_chnl_e eq_chnl);
							 
void bsp_adau1761_eq_save_last(int8_t *eq_status, int8_t *eq_status_last);

//static void SIGMA_SAFELOAD_REGISTER_BLOCK(uint8_t devAddr, uint16_t regAddr, ADI_REG_TYPE length, const ADI_REG_TYPE *pData);
static void SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddr, uint16_t regAddr, uint16_t length, const ADI_REG_TYPE *pData);

#endif
