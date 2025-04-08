#ifndef   _AUDIO_H_
#define  _AUDIO_H_

#include "qcc5125.h"
#include "adau1761.h"


void audio_enable_proc(qcc5125_status_t qcs, uint8_t time);
void audio_download_eqchnl(eq_param_t *eq);
void audio_param_init(eq_param_t *eq);

void audio_save_all_eqchnl_to_eeprom(void);
void audio_read_all_eqchnl_from_eeprom(void);

#endif
