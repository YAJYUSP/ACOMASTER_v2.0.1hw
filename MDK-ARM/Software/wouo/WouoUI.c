#include "WouoUI.h"
#include "stdio.h"
#include "string.h"

// 静态函数声明(方便全局变量初始化函数指针)
void OLED_WinBlurAnimInit(void);
void OLED_WinBlurAnim(uint16_t time);

//--------UI对象类型
window w_all = {0, 0, OLED_WIDTH, OLED_HEIGHT}; // 全局窗口变量，所有的绘制都在这个窗口内进行

//===================================全局变量==================================
// 全局UI参数集合对象(同时初始化)，这个UI的相关参数都在这个集合中定义
UiPara g_default_ui_para = {
    .page_fadeout = true, // 页面退出渐变效果开关
    .win_blur = true,     // 弹窗背景虚化开关
    .ani_param = {
        [IND_ANI] = 60,  // 指示器动画速度
        [TILE_ANI] = 40, // 磁贴动画速度
        [LIST_ANI] = 40, // 列表动画速度
        [WIN_ANI] = 40,  // 弹窗动画速度
        [TAG_ANI] = 40,  // 标签动画速度
        [FADE_ANI] = 10,  // 页面渐变退出速度
        [BLUR_ANI] = 20,  // 弹窗背景模糊速度
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

// 默认UI对象（同时进行初始化）
WouoUI default_ui = {
    .init_finish_flag = false, // 初始时需要先开启过度动画
    .home_page = NULL,         // 初始化当前页面和主页面均是NULL
    .current_page = NULL,
    .state = ui_page_in,         // 从没页面进入主页面，所以是lay_in
    .upara = &g_default_ui_para, // 将默认参数赋值给default_ui
    .msg_queue = {msg_none},
    .msg_que_font = 0, // 消息队列相关的初始化
    .msg_que_rear = 0,
    .indicator = {
        .x = 0,
        .y = 0,
        .w = OLED_WIDTH,
        .h = OLED_HEIGHT,
    },
    .scrollBar = {
        .display = true,
        .y = 0,
        .y_tgt = 0,
    },

#ifdef SOFTWARE_DYNAMIC_REFRESH
    .dynamicDefreshCnt = DYNAMIC_REFRESH_CNT,
#endif

    .lp_var = {.radio_click_flag = false},
    // 滑动数值弹窗
    .slideValWin = {
        .win = {
            .state = win_state_close, // 默认弹窗是关闭的
            .w = SLI_VAL_WIN_W,
            .h = SLI_VAL_WIN_H,
            .l = (OLED_WIDTH - SLI_VAL_WIN_W) >> 1,
            .r = (OLED_WIDTH + SLI_VAL_WIN_W) >> 1,
            .u = (OLED_HEIGHT - SLI_VAL_WIN_H) >> 1,
            .d = (OLED_HEIGHT + SLI_VAL_WIN_H) >> 1,
            .init = OLED_SlideValWinInit,
            .show = OLED_SlideValWinShow,
            .react = OLED_SlideValWinReact, // 关联相关的处理函数
        },
    },
    .confWin = {
        .win = {
            .state = win_state_close, // 默认弹窗是关闭的
            .w = CON_WIN_W,
            .l = (OLED_WIDTH - CON_WIN_W) >> 1,
            .r = (OLED_WIDTH + CON_WIN_W) >> 1,
            .init = OLED_ConWinInit,
            .show = OLED_ConWinShow,
            .react = OLED_ConWinReact, // 关联相关的处理函数
        },
    },
    .infoWin = {
        .win = {
            .state = win_state_close, // 默认弹窗是关闭的
            .w = INFO_WIN_W,
            .l = (OLED_WIDTH - INFO_WIN_W) >> 1,
            .r = (OLED_WIDTH + INFO_WIN_W) >> 1,
            .init = OLED_InfoWinInit,
            .show = OLED_InfoWinShow,
            .react = OLED_InfoWinReact,
        },
    },
    .spinWin = {
        .win = {
            .state = win_state_close, // 默认弹窗是关闭的
            .w = SPIN_WIN_W,
            .h = SPIN_WIN_H,
            .l = (OLED_WIDTH - SPIN_WIN_W) >> 1,
            .r = (OLED_WIDTH + SPIN_WIN_W) >> 1,
            .u = (OLED_HEIGHT - SPIN_WIN_H) >> 1,
            .d = (OLED_HEIGHT + SPIN_WIN_H) >> 1,
            .init = OLED_SpinWinInit,
            .show = OLED_SpinWinShow,
            .react = OLED_SpinWinReact,
        },
    },
};

//--------UI对象类型
WouoUI *p_cur_ui = &default_ui; // 当前操作的ui对象的指针(默认使用defaultui)

/**
 * @brief 将无符号32位整数转换为字符串
 *
 * @param num 要转换的无符号32位整数
 * @return char* 转换后的字符串
 */
char *ui_itoa(uint32_t num) {
    static char str[16] = {0};   // 定义一个静态字符数组，用于存储转换后的字符串
    memset(str, 0, sizeof(str)); // 将字符数组清零
    sprintf(str, "%d", num);     // 将无符号32位整数转换为字符串
    return str;                  // 返回转换后的字符串
}

char *ui_itoa_str(uint32_t num, char *str) {
    memset(str, 0, sizeof(str)); // 将字符数组清零
    sprintf(str, "%d", num);     // 将无符号32位整数转换为字符串
    return str;                  // 返回转换后的字符串
}

// /**
//  * @brief 将浮点数转换为字符串
//  *
//  * @param num 要转换的浮点数
//  * @return char* 转换后的字符串
//  */
// char *ui_ftoa(float num) {
//     static char str[16] = {0};   // 定义一个静态字符数组，用于存储转换后的字符串
//     memset(str, 0, sizeof(str)); // 将字符数组清零
//     sprintf(str, "%.2f", num);   // 将浮点数转换为字符串，并保留两位小数
//     return str;                  // 返回转换后的字符串
// }

/**
 * @brief 将浮点数转换为字符串
 *
 * @param num 要转换的数
 * @param decimalNum 小数点位数
 * @return char* 转换后的字符串
 */
char *ui_ftoa(int32_t num, DecimalNum decimalNum) {
    static char str[16] = {0};   // 定义一个静态字符数组，用于存储转换后的字符串
    memset(str, 0, sizeof(str)); // 将字符数组清零
    switch (decimalNum) {
    case DecimalNum_0:
        sprintf(str, "%d", num);
        break;
    case DecimalNum_1:
        sprintf(str, "%g", num / 10.0f);
        break;
    case DecimalNum_2:
        sprintf(str, "%g", num / 100.0f);
        break;
    case DecimalNum_3:
        sprintf(str, "%g", num / 1000.0f);
        break;
    default:
        break;
    }
    return str; // 返回转换后的字符串
}

/**
 * @brief 将浮点数转换为字符串
 *
 * @param num 要转换的数
 * @param decimalNum 小数点位数
 * @param str 转换后的字符串
 * @return char* 转换后的字符串
 */
char *ui_ftoa_str(int32_t num, DecimalNum decimalNum, char *str) {
    memset(str, 0, sizeof(str)); // 将字符数组清零
    switch (decimalNum) {
    case DecimalNum_0:
        sprintf(str, "%d", num);
        break;
    case DecimalNum_1:
        sprintf(str, "%g", num / 10.0f);
        break;
    case DecimalNum_2:
        sprintf(str, "%g", num / 100.0f);
        break;
    case DecimalNum_3:
        sprintf(str, "%g", num / 1000.0f);
        break;
    default:
        break;
    }
    return str; // 返回转换后的字符串
}

/**
 * @brief 获取Option项目的浮点值
 * 
 * @param option 目标项目
 * @return float val的浮点值
 */
float OLED_GetOptionFloatVal(Option *option) {
    switch (option->decimalNum) {
    case DecimalNum_0:
        return option->val;
        break;
    case DecimalNum_1:
        return option->val / 10.0f;
        break;
    case DecimalNum_2:
        return option->val / 100.0f;
        break;
    case DecimalNum_3:
        return option->val / 1000.0f;
        break;
    default:
        break;
    }
}

#ifdef SOFTWARE_DYNAMIC_REFRESH
void OLED_UIBeginDynamicRefresh(void) {
    p_cur_ui->dynamicDefreshCnt = DYNAMIC_REFRESH_CNT;
}
#endif

/**
 * @brief 向msg队列发送消息
 *
 * @param msg 消息
 */
void OLED_MsgQueSend(InputMsg msg) {
    if (!OLED_MsgQueIsFull) {
        p_cur_ui->msg_queue[p_cur_ui->msg_que_rear] = msg;
        p_cur_ui->msg_que_rear++;
        if (p_cur_ui->msg_que_rear == INPUT_MSG_QUNEE_SIZE)
            p_cur_ui->msg_que_rear = 0;
    }
#ifdef SOFTWARE_DYNAMIC_REFRESH
    OLED_UIBeginDynamicRefresh();
#endif
}

/**
 * @brief 读msg队列消息
 *
 * @return InputMsg 消息
 */
InputMsg OLED_MsgQueRead(void) {
    InputMsg msg = msg_none;
    if (!OLED_MsgQueIsEmpty) {
        msg = p_cur_ui->msg_queue[p_cur_ui->msg_que_font];
        p_cur_ui->msg_que_font++;
        if (p_cur_ui->msg_que_font == INPUT_MSG_QUNEE_SIZE)
            p_cur_ui->msg_que_font = 0;
    }
    return msg;
}

/**
 * @brief 清空msg队列消息
 */
bool OLED_MsgQueClear(void) {
    p_cur_ui->msg_que_font = 0;
    p_cur_ui->msg_que_rear = 0;
    return OLED_MsgQueIsEmpty;
}

//-------UI相关函数
// 将当前操作的UI对象选择为默认UI对象
void OLED_SelectDefaultUI(void) {
    p_cur_ui = &default_ui;
}
// 设置当前正在操作的UI对象
void OLED_SetCurrentUI(WouoUI *ui) {
    p_cur_ui = ui;
}
// 初始化一个新的UI对象，需要传入这个UI的设置参数
//(这个还需要改进,只是初始化一个带参数的UI对象没有意义，需要带上对应的画点函数才有意义)
void OLED_NewUI(WouoUI *ui, UiPara *ui_para) {
    *ui = default_ui;    // 将默认UI的值复制到新UI
    ui->upara = ui_para; // 将参数设置为新的UI的参数
}

/**
 * @brief 页面进入初始化
 *
 * @param time 轮询时间
 */
void OLED_UILayerInProc(uint16_t time) {
    if (p_cur_ui->current_page != NULL) // 检查该页面地址存在
    {
        Page *p = (Page *)(p_cur_ui->current_page); // 把当前页面转为page读取
        if (p->init != NULL)
            p->init(p_cur_ui->current_page, time);
    }
}

/**
 * @brief 页面渐变退出动画处理
 *
 * @param time 轮询时间
 */
void OLED_UILayerOutProc(uint16_t time) {
    OLED_UIBlurProc(time);                                       // 页面渐变动画处理
    if (p_cur_ui->Uiblur.blur_end) {                             // 页面渐变消失完成
        p_cur_ui->current_page = p_cur_ui->in_page;              // 将UI页面设置为上一级页面
        p_cur_ui->init_finish_flag = false;                      // 启用过度页面动画
        p_cur_ui->state = ui_page_in;                            // 页面渐变消失完成后，进行页面渐变出现
        OLED_UIFadeInInit(p_cur_ui->upara->ani_param[FADE_ANI]); // 背景页面渐变出现
    }
}

/**
 * @brief UI画面渐变消失动画初始化
 *
 * @param blur_tgt 目标渐变程度：0-4
 * @param blur_time 渐变动画间隔时间
 */
void OLED_UIFadeOutInit(uint8_t blur_tgt, uint16_t blur_time) {
    p_cur_ui->Uiblur.blur_mode = UIBlur_FadeOut;
    p_cur_ui->Uiblur.blur_tgt = blur_tgt;
    p_cur_ui->Uiblur.blur_end = false;
    p_cur_ui->Uiblur.blur_time = blur_time;
    p_cur_ui->Uiblur.timer = 0;
}

/**
 * @brief UI画面渐变出现动画初始化
 *
 * @param blur_time 渐变动画间隔时间
 */
void OLED_UIFadeInInit(uint16_t blur_time) {
    p_cur_ui->Uiblur.blur_mode = UIBlur_FadeIn;
    p_cur_ui->Uiblur.blur_tgt = 0;
    p_cur_ui->Uiblur.blur_end = false;
    p_cur_ui->Uiblur.blur_time = blur_time;
    p_cur_ui->Uiblur.timer = 0;
}

/**
 * @brief UI画面渐变动画处理函数
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIBlurProc(uint16_t time) {
    if (p_cur_ui->Uiblur.timer <= p_cur_ui->Uiblur.blur_time) {
        p_cur_ui->Uiblur.timer += time;
        OLED_MsgQueClear();     // 防止页面切换时按键消息输入导致动画冲突
#ifdef SOFTWARE_DYNAMIC_REFRESH // 软件动态刷新
        p_cur_ui->dynamicDefreshCnt = DYNAMIC_REFRESH_CNT;
#endif
    } else {
        if (p_cur_ui->Uiblur.blur_mode == UIBlur_FadeOut &&
            p_cur_ui->Uiblur.blur_cur < p_cur_ui->Uiblur.blur_tgt) {
            p_cur_ui->Uiblur.blur_cur++;
            p_cur_ui->Uiblur.timer = 0;
        }
        if (p_cur_ui->Uiblur.blur_mode == UIBlur_FadeIn &&
            p_cur_ui->Uiblur.blur_cur > p_cur_ui->Uiblur.blur_tgt) {
            p_cur_ui->Uiblur.blur_cur--;
            p_cur_ui->Uiblur.timer = 0;
        }
    }
    p_cur_ui->Uiblur.blur_end = (p_cur_ui->Uiblur.blur_cur == p_cur_ui->Uiblur.blur_tgt);

    OLED_SetPointColor(0);
    switch (p_cur_ui->Uiblur.blur_cur) {
    case 0:
        break;
    case 1:
        OLED_AllBlur(BLUR_1_4);
        break;
    case 2:
        OLED_AllBlur(BLUR_2_4);
        break;
    case 3:
        OLED_AllBlur(BLUR_3_4);
        break;
    case 4:
        OLED_AllBlur(BLUR_4_4);
        break;
    default:
        break;
    }
    OLED_SetPointColor(1);
}


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

/**
 * @brief 指示器绘制
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIIndicatorProc(uint8_t time) {
    // indicator
    OLED_Animation(&(p_cur_ui->indicator.x), &(p_cur_ui->indicator.x_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.y), &(p_cur_ui->indicator.y_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.w), &(p_cur_ui->indicator.w_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.h), &(p_cur_ui->indicator.h_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);

    PageType pageType = OLED_CheckPageType(p_cur_ui->current_page);

    // slideValWin
    if (p_cur_ui->slideValWin.win.state == win_state_show) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, SLI_VAL_WIN_BAR_R);
        OLED_SetPointColor(1); // 实色绘制
    }
    // confWin
    else if (p_cur_ui->confWin.win.state == win_state_show) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, CON_WIN_BTN_R);
        OLED_SetPointColor(1); // 实色绘制
    }
    // infoWin
    else if (p_cur_ui->infoWin.win.state == win_state_show) {
        OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                              p_cur_ui->indicator.w, p_cur_ui->indicator.h, INFO_WIN_R);
    }
    // spinWin
    else if (p_cur_ui->spinWin.win.state == win_state_show) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, 0);
        OLED_SetPointColor(1); // 实色绘制
    } else {
        switch (pageType) {
        case type_title:
            OLED_SetPointColor(2); // 反色绘制
            OLED_WinDrawBoxRightAngle(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                                      p_cur_ui->indicator.w, p_cur_ui->indicator.h, TILE_ICON_IND_R);
            OLED_SetPointColor(1); // 实色绘制
            break;
        case type_list:
            if (p_cur_ui->slideValWin.win.state == win_state_close &&
                p_cur_ui->confWin.win.state == win_state_close &&
                p_cur_ui->infoWin.win.state == win_state_close) {
                OLED_SetPointColor(2); // 反色绘制
                OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                                 p_cur_ui->indicator.w, p_cur_ui->indicator.h, LIST_BOX_R);
                OLED_SetPointColor(1); // 实色绘制
            }
            break;
        case type_wave:
            break;
        default:
            break;
        }
    }
}

/**
 * @brief 进度条绘制
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIScrollBarProc(uint8_t time) {
    // Animation
    OLED_Animation(&(p_cur_ui->scrollBar.y), &(p_cur_ui->scrollBar.y_tgt), p_cur_ui->upara->ani_param[LIST_ANI], time);

    PageType pageType = OLED_CheckPageType(p_cur_ui->current_page);
    switch (pageType) {
    case type_title:
        break;
    case type_list:
        OLED_WinDrawHLine(&w_all, OLED_WIDTH - LIST_BAR_W, OLED_WIDTH, 0);
        OLED_WinDrawHLine(&w_all, OLED_WIDTH - LIST_BAR_W, OLED_WIDTH, OLED_HEIGHT - 1);
        OLED_WinDrawVLine(&w_all, OLED_WIDTH - ((LIST_BAR_W / 2) + 1), 0, OLED_HEIGHT);
        OLED_WinDrawRBox(&w_all, OLED_WIDTH - LIST_BAR_W, 0, LIST_BAR_W, p_cur_ui->scrollBar.y, 0);
        break;
    case type_wave:
        break;
    default:
        break;
    }
}

/**
 * @brief UI总任务(FSM)
 *
 * @param time 函数轮询间隔时间
 */
void OLED_UIProc(uint8_t time) {
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

#ifdef HARDWARE_DYNAMIC_REFRESH
        if (memcmp(oled_buff_dynamic, oled_buff, sizeof(oled_buff))) {
            memcpy(oled_buff_dynamic, oled_buff, sizeof(oled_buff));
            OLED_SendBuff();
        }
#else
    OLED_SendBuff();
#endif
#ifdef SOFTWARE_DYNAMIC_REFRESH
    }
#endif
}

/**
 * @brief 跳转到指定页面(跳转是会将当前页面id输入，用于关联确认上下级页面关系)
 *
 * @param self_page 当前页面对象的地址
 * @param terminate_page 目标页面地址
 * 从一个页面跳转到另一个页面，常用于回调函数中调用，并确定页面的上下级关系
 * 这样，在terminate_page页面收到return消息时，会返回self_page_id所代表的页面
 */
void OLED_UIJumpToPage(PageAddr self_page_addr, PageAddr terminate_page) {
    // 关联上级页面并跳转页面
    if (terminate_page != NULL) {
        Page *p_ter = (Page *)terminate_page;
        p_ter->last_page = self_page_addr;
        // 跳转页面时，如果当前页面有弹窗，将其关闭
        p_cur_ui->slideValWin.win.state = win_state_close;
        p_cur_ui->spinWin.win.state = win_state_close;
        p_cur_ui->confWin.win.state = win_state_close;
        p_cur_ui->infoWin.win.state = win_state_close;
        p_cur_ui->state = ui_page_out;                               // 开始层级渲染
        p_cur_ui->in_page = terminate_page;                          // 将UI页面设置为上一级页面
        OLED_UIFadeOutInit(4, p_cur_ui->upara->ani_param[FADE_ANI]); // 页面渐变消失动画初始化
    }
}

/**
 * @brief 改变当前页面(不会关联上下级页面关系，只是单纯跳转页面)
 *
 * @param terminate_page The address of the page to change to.
 */
void OLED_UIChangeCurrentPage(PageAddr terminate_page) {
    if (terminate_page != NULL) {
        // 跳转页面时，如果当前页面有弹窗，将其关闭
        p_cur_ui->slideValWin.win.state = win_state_close;
        p_cur_ui->spinWin.win.state = win_state_close;
        p_cur_ui->confWin.win.state = win_state_close;
        p_cur_ui->infoWin.win.state = win_state_close;
        p_cur_ui->state = ui_page_out;                               // Start rendering the layers.
        p_cur_ui->in_page = terminate_page;                          // 将UI页面设置为上一级页面
        OLED_UIFadeOutInit(4, p_cur_ui->upara->ani_param[FADE_ANI]); // 页面渐变消失动画初始化
    }
}

/**
 * @brief 得到当前页面的地址
 *
 * @param terminate_page The address of the page to change to.
 */
Page *OLED_GetCurrentPage(void) {
    return (Page *)p_cur_ui->current_page;
}

/**
 * @brief 非线性运动函数
 *
 * @param a 当前值
 * @param a_tgt 目标值
 * @param aniTime 运动时间
 * @param time 轮序间隔时间
 */
void OLED_Animation(float *a, float *a_tgt, uint16_t aniTime, uint16_t time) {
    if (*a != *a_tgt) {
        if (fabs(*a - *a_tgt) < 0.25)
            *a = *a_tgt;
        else
            *a += (*a_tgt - *a) / (aniTime / time); // 慢慢接近
#ifdef SOFTWARE_DYNAMIC_REFRESH
        OLED_UIBeginDynamicRefresh();
#endif
    }
}
