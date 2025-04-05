# 移植

## OLED底层驱动
1.首先确保你已经实现了OLED底层驱动，即类似
```C
//将一帧显存数组更新到OLED,要求显存结构为uint8_t buff[8][128]
void OLED_Refresh_FrameBuffer(uint8_t buff[][128]);
```
## 显示部分的移植
1.在WouoUI_port.c中实现下列函数
```C
/* 显示初始化函数，添加代码可以实现在初始化WouoUI的时候对屏幕进行初始化*/
void WouoUI_port_Init(void) {}

/* 帧刷新函数，WouoUI使用这个函数来对屏幕进行全局刷新*/
void WouoUI_port_SendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]) {
    OLED_Refresh_FrameBuffer(buff);
}
```
## 外部输入的处理
WouoUI靠
```C
void OLED_MsgQueSend(InputMsg msg)；
```
函数来读取外部输入。其中InputMsg可以为
```C
typedef enum {
    msg_none = 0x00, // none表示没有操作
    msg_add,         // 加，用于数值调整
    msg_sub,         // 减，用于数值调整
    msg_up,          // 上，或者last消息，表上一个
    msg_down,        // 下，或者next消息，表下一个
    msg_click,       // 点击消息，表确认，确认某一选项，回调用一次回调
    msg_return,      // 返回消息，表示返回，从一个页面退出
    msg_home,        // home消息，表回主界面(尚未设计，目前还没有设计对应的功能，默认以page_id为0的页面为主页面)
} InputMsg;          // 输入消息类型，UI设计只供输入5种消息
```
## 用户偏好与外观配置
在WouoUI.c中修改动画速度与画面参数：
```C
// 全局UI参数集合对象(同时初始化)，这个UI的相关参数都在这个集合中定义
UiPara g_default_ui_para = {
    .page_fadeout = true, // 页面退出渐变效果开关
    .win_blur = true,     // 弹窗背景虚化开关
    .ani_param = {
        [IND_ANI] = 120,  // 指示器动画速度
        [TILE_ANI] = 100, // 磁贴动画速度
        [LIST_ANI] = 100, // 列表动画速度
        [WIN_ANI] = 100,  // 弹窗动画速度
        [TAG_ANI] = 100,  // 标签动画速度
        [FADE_ANI] = 20,  // 页面渐变退出速度
        [BLUR_ANI] = 40,  // 弹窗背景模糊速度
    },
    .ufd_param = {
        [TILE_UFD] = false, // 磁贴图标从头展开开关
        [LIST_UFD] = false, // 菜单列表从头展开开关
    },
    .loop_param = {
        [TILE_LOOP] = true, // 磁贴图标循环模式开关
        [LIST_LOOP] = true, // 菜单列表循环模式开关
    },
};
```
## 运行示例
1.调用WouoUI_user.c中的初始化函数：
```C
TestUI_Init()；
2.开启定时任务作为WouoUI的心跳，这里以20ms周期为例：
```C
//PIT
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t time_cnt_20ms = 0;
    //5ms period 
    if (htim == (&htim21))                      
    {
      //20ms定时中断，用于显示刷新
      if(time_cnt_20ms < 3)
        time_cnt_20ms ++;
      else
      {
        time_cnt_20ms = 0;
        OLED_UIProc(20);
      }
    }
}
```

# 开发

## 用户自定义页面

WouoU没有控件的概念，而用页面作为最基本的交互单元
```C
struct _page {
    PageType page_type; // 页面类型，以便在处理时调用不同函数绘制
    PageAddr last_page; // 父页面的地址
    CallBackFunc cb;    // 页面的回调函数
    PageInit init;      // 进入页面动画初始化方法
    PageShow show;      // 页面的展示函数
    PageReact react;    // 页面的响应函数
}; // 最基本的页面类型(所有页面类型的基类和结构体的**第一个成员**)
```
目前支持的页面种类有
```C
//--------页面类型枚举
typedef enum {
    type_title = 0x00, // 磁贴类
    type_list,         // 列表类
                       // 终端类
    type_wave,         // 波形显示类
		type_userdef,  // 用户自定义类
} PageType;            // 页面类型，用于标志传入的每个页面类型，方便调用对应的proc函数
```
如果用户想要使用用户自定义页面，即：为页面自定义样式、交互方式、回调函数等功能，

1.需要在WouoUI_udp.c中实现自定义页面的以下几种方法
```C
void OLED_CustomPageEnterInit(PageAddr page_addr, uint16_t time) {}   // 进入页面时调用一次，用于初始化页面相关的各种自定义参数
void OLED_CustomPageShow(PageAddr page_addr, uint16_t time) {}        // 绘制此页面的基本UI框架，在进入页面后会一直执行 
void OLED_CustomPageReact(PageAddr page_addr, uint16_t time) {}       // 供用户实现自定义页面的功能、交互方式、动画、调用回调函数、进入/返回其他页面等基本业务
void OLED_CustomPageInit(                                             // 定义此自定义页面结构体所需的各种参数、函数指针、选项列表等
    CustomPage *cstm_page,  // 磁贴页面对象
    uint8_t item_num,       // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    Icon *icon_array,       // 整个页面的icon数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
{}
```
用户可以参考WouoUI_udp.c中的PlayingPage示例理解各个方法的作用

2.需要在WouoUI_user.c中实现各个页面的选项列表、图标列表、回调函数以及初始化方法.以main页面为例，
```C
// main页面的选项
const Option mian_option_array[MAIN_PAGE_NUM] = {}
// main页面的图标
const Icon main_icon_array[MAIN_PAGE_NUM] = {}
// main页面的回调函数，主要用于页面跳转
void MainPage_CallBack(const Page *cur_page_addr, Option *select_item) {}
// 页面初始化函数，供主函数调用
void TestUI_Init(void) {}
```

## WouoUI的渲染逻辑
WouoUI的核心渲染逻辑体现在WouoUI.c的下面两个函数中：
```C
/**
 * @brief UI总任务(FSM)
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIProc(uint8_t time) {
	
	Page *p = (Page *)(p_cur_ui->current_page);
	PageType pt = OLED_CheckPageType(p_cur_ui->current_page);
	
	/* 如果目前是用户自定义页面，那么页面处理流程会有不同*/
	if(pt == type_userdef){
		
		p_cur_ui->dynamicDefreshCnt = 0;
		switch (p_cur_ui->state) // ui状态机轮询
		{
		case ui_page_out:
				OLED_ClearBuff();           // 清空buff
				OLED_UIPageProc(time);      // 绘制页面
				OLED_UILayerOutProc(time);  // 绘制页面渐变
				OLED_UIScrollBarProc(time); // 绘制进度条
				break;
		case ui_page_in: // 主要是对页面动画在切换时做一次参数的赋值
				OLED_UILayerInProc(time);
				p_cur_ui->state = ui_page_proc;
				p->show(p_cur_ui->current_page, time); // 页面show
				break;
		case ui_page_proc:
				/* 对于用户自定义页面，为了能够实现用户自定义的动画、交互等功能，需要反复处理页面的react函数*/
				/* (WouoUI应该是把页面的Show函数视为一个页面的UI框架绘制，而且每次刷新屏幕时都会清空屏幕并且重新绘制框架)*/
				OLED_ClearBuff();           					 // 清空buff
				p->show(p_cur_ui->current_page, time); // 页面show
				p->react(p_cur_ui->current_page, time);
				break;
		default:
				break;
		}
	}
	
	else{
		
		/* 下面是对WouoUI默认支持的页面类型的处理*/
		#ifdef SOFTWARE_DYNAMIC_REFRESH
    if (p_cur_ui->dynamicDefreshCnt) {
        p_cur_ui->dynamicDefreshCnt--;
		#endif
			
			switch (p_cur_ui->state) // ui状态机轮询
			{
			case ui_page_out:
					OLED_ClearBuff();           // 清空buff
					OLED_UIPageProc(time);      // 绘制页面
					OLED_UILayerOutProc(time);  // 绘制页面渐变
					OLED_UIScrollBarProc(time); // 绘制进度条
					break;
			case ui_page_in: // 主要是对页面动画在切换时做一次参数的赋值
					OLED_UILayerInProc(time);
					p_cur_ui->state = ui_page_proc;
					break;
			case ui_page_proc:
					OLED_ClearBuff();           // 清空buff
					OLED_UIPageProc(time);      // 绘制页面以及弹窗
					OLED_UIScrollBarProc(time); // 绘制进度条
					break;
			default:
					break;
			}
			OLED_UIIndicatorProc(time); // 绘制指示器
		}
	}
	
	/* 动态刷新，如果显存一致则不进行刷新*/
#ifdef HARDWARE_DYNAMIC_REFRESH
        if (memcmp(oled_buff_dynamic, oled_buff, sizeof(oled_buff))) {
            memcpy(oled_buff_dynamic, oled_buff, sizeof(oled_buff));
            OLED_SendBuff();
        }
#else
    OLED_SendBuff();
#endif
#ifdef SOFTWARE_DYNAMIC_REFRESH
		
#endif
}
```
```C
/**
 * @brief 页面处理任务函数
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIPageProc(uint16_t time) {
    // 页面绘制
    if (p_cur_ui->current_page != NULL) // 该页面地址存在
    {
        // 判断页面类型并赋值
        Option *op = NULL;
        Page *p = NULL;
        TitlePage *cur_tile = NULL;
        ListPage *cur_list = NULL;
        PageType pt = OLED_CheckPageType(p_cur_ui->current_page);
        switch (pt) {
        case type_title:
            cur_tile = (TitlePage *)(p_cur_ui->current_page);
            op = &(cur_tile->option_array[cur_tile->select_item]);
            break;
        case type_list:
            cur_list = (ListPage *)(p_cur_ui->current_page);
            op = &(cur_list->option_array[cur_list->select_item]);
            break;
        default:
            break;
        }
        // 进行页面show
        p = (Page *)(p_cur_ui->current_page);  // 把当前页面转为page读取
        p->show(p_cur_ui->current_page, time); // 页面show
        OLED_UIBlurProc(time);                 // UI渐变处理
        // 页面内：无弹窗：页面react；有弹窗：弹窗show、react
        if (pt == type_list || pt == type_title) {
            if (p_cur_ui->slideValWin.win.state == win_state_close &&
                p_cur_ui->spinWin.win.state == win_state_close &&
                p_cur_ui->confWin.win.state == win_state_close &&
                p_cur_ui->infoWin.win.state == win_state_close) {
                p->react(p_cur_ui->current_page, time);
            }
            OLED_WinFSM(&(p_cur_ui->slideValWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->confWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->infoWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->spinWin.win), p_cur_ui->current_page, op, time);
        } else
            p->react(p_cur_ui->current_page, time);
    }
}
```
其中OLED_UIProc函数为了实现用户自定义页面做过改动。
值得注意的是，WouoUI把0像素点视为“透明像素”而不是“黑色”，所以渲染逻辑会与直接驱动OLED不同，用户可自行参考源码。