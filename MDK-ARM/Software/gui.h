#ifndef   _GUI_H_
#define  _GUI_H_

#define xx 0
#define yy 1

#include "pwr_mngr.h"
#include "adau1761.h"


#define LIGHTNING_DISP_X 			  89
#define BATT_DISP_X  						108
#define SOC_DISP_X  						91
#define BLUETOOTH_DISP_X  			60


void GUI_powerUp(void);
void GUI_power_off(void);
void GUI_wait_until_connect(void);
void GUI_main_page(void);
void GUI_update_status_bar(sys_chg_t *chg_info, uint8_t force_rfs);
void GUI_update_button(void);
void GUI_eq(int8_t *eq_status);
void GUI_oled_draw_wave(int8_t *eq_status, int8_t *eq_status_last);

void gui_update_eq_freq(eq_chnl_e current_channel, eq_chnl_e last_channel);
void gui_update_eq_line(int8_t *eq_status, eq_chnl_e current_channel, eq_chnl_e last_channel);
void gui_update_eq_boost(void);

extern eq_chnl_e select_eq, select_eq_last;
extern uint8_t GUI_OLEDRFS_REQ;


#endif
