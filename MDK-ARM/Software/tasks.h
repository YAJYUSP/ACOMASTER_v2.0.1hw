#ifndef   _TASKS_H_
#define  _TASKS_H_

typedef enum
{
		SYSTEM_INIT = 			0x00,									//开机初始化程序
		INIT_UI = 					0x03,									//开机初始化界面UI
		BLUETOOTH_CONNECT = 0x01,									//等待蓝牙连接界面
		MAIN_UI = 					0x02,									//主页面（播放页面）			
		EQ_UI_TRANSITION =  0x04,									//EQ页面过渡画面	
		EQ_UI = 						0x05,									//EQ页面
		EQ_SAVING_UI = 			0x06,									//EQ保存页面
}device_state_e;

extern device_state_e device_current_state;
extern device_state_e device_last_state;


void main_FSM(device_state_e *DeviceState, device_state_e *Device_LastState, uint8_t TaskPeriod);

#endif
