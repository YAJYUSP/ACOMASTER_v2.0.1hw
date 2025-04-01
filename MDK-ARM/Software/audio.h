#ifndef   _AUDIO_H_
#define  _AUDIO_H_

extern uint8_t audio_init_flag;

void audio_enable(void);
void audio_pulse(void);
void audio_unmute(void);
void audio_mute(void);


#endif
