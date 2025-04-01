#ifndef   _PWR_MNGR_H_
#define  _PWR_MNGR_H_

// 按键睡眠超时时间         （n*100ms）
#define KEY_SLEEP_TIMEOUT_TIME        300
// 无播放时自动关机超时时间 
// 休眠时的10s周期RTC中断，超过n次没有检测到音频播放，就自动关机（10s*n）
#define DEVICE_PWROFF_TIMEOUT_TIME    60

typedef enum
{
		//亮屏（正常运行）
		SCREEN_ON,
		//息屏（音乐照常播放，屏幕熄灭，单片机进入省电模式）
		SCREEN_OFF,
		//充电
		CHARGING,
		//开机中
		POWER_ON,
		//关机
		POWER_OFF,
}sys_status_e;

typedef enum
{
		I2C_BUSY,
		I2C_IDLE,
}i2c_bus_status_e;

typedef struct
{
		//当前系统状态，通过改变该值来切换设备状态
		sys_status_e sys_status;
		//上一次系统状态
		sys_status_e sys_status_last;
}sys_ctrl_t;

typedef struct
{
		//电池电压
		float battery_volt;
		//电量百分比
		uint8_t soc, soc_last;
		//剩余运行时间
		uint16_t remain_rt;

		//当前充电状态，置1代表正在充电
		uint8_t chrg_state, chrg_state_last;
		/* 充电时显示电池logo动画效果 */
		uint8_t batt_soc_animation_state,batt_soc_animation_state_last;
}sys_chg_t;



extern sys_chg_t sys_chg_info;
extern sys_ctrl_t sys_ctrl;

void Configure_GPIO_Floating(void);

void sys_chg_info_init(sys_chg_t *chg_info);
void sys_chg_updt_info(sys_chg_t *chg_info);

void sys_ctrl_status_switch(sys_ctrl_t *ctrl);

void sys_pwrctrl_power_up(void);
void sys_pwrctrl_power_down(void);

#endif
