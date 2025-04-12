#ifndef _ADAU1761_H_
#define _ADAU1761_H_

#include "adau1761_REG.h"

#define DEVICE_ARCHITECTURE_IC_2                  "ADAU176x"
#define DEVICE_ADDR_IC_2                          0x70

#define EQ_BOOST_MAX  20

typedef unsigned char ADI_REG_TYPE;

// 10段EQ的中心频率，用于索引
typedef enum
{
		freq_63, freq_125, freq_250, freq_500, freq_1k,
		freq_2k, freq_4k, freq_8k, freq_12k, freq_16k
}eq_cfreq_e;

typedef struct
{
		//boost of filter
		int8_t eq_boost;
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
}eq_param_t;


void default_download_IC_2(void);
void bsp_adau1761_eq_download(eq_param_t *eq);




#endif
