#include "WouoUI_win.h"
#include "WouoUI.h"

/**
 * @brief OLED_WinFSM函数用于处理窗口的状态机
 *
 * @param w 指向窗口结构体的指针
 * @param bg 页面地址
 * @param sel_item 指向选项结构体的指针
 * @param time 状态机轮询间隔时间
 */
void OLED_WinFSM(Win *w, PageAddr bg, Option *sel_item, uint16_t time) {
    switch (w->state) {
    case win_state_close:
        break;
    case win_state_init: // 初始化只用在弹窗最开始时做—次弹窗参数初始化
        w->init(bg, sel_item);
        w->state = win_state_show;
        break;
    case win_state_show: // 弹窗动画和交互
        w->show(sel_item, time);
        w->react(bg, sel_item);
        break;
    default:
        break;
    }
}

//------------ 滑动数值弹窗相关函数
void OLED_SlideValWinInit(PageAddr bg, Option *sel_item) {
    UNUSED_PARAMETER(bg);
    // 窗口y坐标
    p_cur_ui->slideValWin.win.y = SLI_VAL_WIN_Y_TGT;
    p_cur_ui->slideValWin.win.y_tgt = p_cur_ui->slideValWin.win.u;
    // 进度条参数
    p_cur_ui->slideValWin.slider_l = (OLED_WIDTH - SLI_VAL_WIN_BAR_W) / 2;
    p_cur_ui->slideValWin.slider_r = (OLED_WIDTH + SLI_VAL_WIN_BAR_W) / 2;
    p_cur_ui->slideValWin.slider_u = (OLED_HEIGHT - SLI_VAL_WIN_H) / 2 + SLI_VAL_WIN_BAR_Y_OFS;
    p_cur_ui->slideValWin.slider_d = (OLED_HEIGHT - SLI_VAL_WIN_H) / 2 + SLI_VAL_WIN_BAR_Y_OFS + SLI_VAL_WIN_BAR_H;
    // 提示字符串
    ui_itoa_str(sel_item->min, p_cur_ui->slideValWin.min_val_str);
    ui_itoa_str(sel_item->max, p_cur_ui->slideValWin.max_val_str);
    p_cur_ui->slideValWin.min_val_w = OLED_GetStrWidth(p_cur_ui->slideValWin.min_val_str, SLI_VAL_WIN_FONT);
    // 背景页面渐变消失
    OLED_UIFadeOutInit(2, p_cur_ui->upara->ani_param[BLUR_ANI]);
}

void OLED_SlideValWinShow(Option *sel_item, uint16_t time) {
    // 窗口y坐标运动
    OLED_Animation(&p_cur_ui->slideValWin.win.y, &p_cur_ui->slideValWin.win.y_tgt, p_cur_ui->upara->ani_param[WIN_ANI], time);
    // 弹窗填充
    OLED_SetPointColor(0);
    OLED_WinDrawRBox(&w_all, p_cur_ui->slideValWin.win.l, (int16_t)p_cur_ui->slideValWin.win.y,
                     SLI_VAL_WIN_W, SLI_VAL_WIN_H, SLI_VAL_WIN_R);
    // 弹窗外框
    OLED_SetPointColor(1);
    OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->slideValWin.win.l, (int16_t)p_cur_ui->slideValWin.win.y,
                          SLI_VAL_WIN_W, SLI_VAL_WIN_H, SLI_VAL_WIN_R);
    // 进度条外框
    OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->slideValWin.slider_l, (int16_t)p_cur_ui->slideValWin.win.y + SLI_VAL_WIN_BAR_Y_OFS,
                          SLI_VAL_WIN_BAR_W, SLI_VAL_WIN_BAR_H, SLI_VAL_WIN_BAR_R);
    // 提示文本跳过“~ ”
    OLED_WinDrawStr(&w_all, p_cur_ui->slideValWin.win.l + FONT_MARGIN,
                    (int16_t)p_cur_ui->slideValWin.win.y + FONT_MARGIN, WIN_FONT, (uint8_t *)&(sel_item->text[2]));
    // 当前数值
    OLED_WinDrawStr(&w_all, p_cur_ui->slideValWin.win.r - FONT_MARGIN - OLED_GetStrWidth(ui_itoa(sel_item->val), WIN_FONT),
                    (int16_t)p_cur_ui->slideValWin.win.y + FONT_MARGIN, WIN_FONT, (uint8_t *)(ui_itoa(sel_item->val)));
    // 最小值
    OLED_WinDrawStr(&w_all, p_cur_ui->slideValWin.slider_l - 2 - p_cur_ui->slideValWin.min_val_w,
                    (int16_t)p_cur_ui->slideValWin.win.y + 20, SLI_VAL_WIN_FONT, (uint8_t *)p_cur_ui->slideValWin.min_val_str);
    // 最大值
    OLED_WinDrawStr(&w_all, p_cur_ui->slideValWin.slider_r + 2,
                    (int16_t)p_cur_ui->slideValWin.win.y + 20, SLI_VAL_WIN_FONT, (uint8_t *)p_cur_ui->slideValWin.max_val_str);
    // indicator 进度条长度由指示器绘制
    p_cur_ui->indicator.x_tgt = p_cur_ui->slideValWin.slider_l + 2;
    p_cur_ui->indicator.y_tgt = p_cur_ui->slideValWin.slider_u + 2;
    p_cur_ui->indicator.w_tgt = (sel_item->val - sel_item->min) * (SLI_VAL_WIN_BAR_W - 4) / (sel_item->max - sel_item->min);
    p_cur_ui->indicator.h_tgt = SLI_VAL_WIN_BAR_H - 4;
}

void OLED_SlideValWinReact(PageAddr bg, Option *sel_item) {
    // if (p_cur_ui->slideValWin.win.y == p_cur_ui->slideValWin.win.y_tgt && p_cur_ui->slideValWin.win.y != SLI_VAL_WIN_Y_TGT) // 弹窗进场动画结束时
    if (p_cur_ui->slideValWin.win.y == p_cur_ui->slideValWin.win.y_tgt) // 弹窗进场动画结束时
    {
        InputMsg msg = OLED_MsgQueRead();
        switch (msg) {
        case msg_add:
            if (sel_item->val + sel_item->step <= sel_item->max)
                sel_item->val += sel_item->step;
            break;
        case msg_sub:
            if (sel_item->val - sel_item->step >= sel_item->min)
                sel_item->val -= sel_item->step;
            break;
        case msg_up:
        case msg_down:
            break;
        case msg_click: // 点击确认也会退出弹窗，不过，点击确认是会触发一次回调，点击返回则不会
        case msg_return:
            // p_cur_ui->slideValWin.win.y_tgt = SLI_VAL_WIN_Y_TGT;     // 弹窗退场
            OLED_UIFadeInInit(p_cur_ui->upara->ani_param[BLUR_ANI]); // 背景页面渐变出现
            p_cur_ui->slideValWin.win.state = win_state_close;       // 状态标志，退出弹窗
            break;
        case msg_home:
            return;
        default:
            break;
        }
        if (msg != msg_none && msg != msg_return) { // 只有弹窗内有动作，就会实时触发回调
            Page *p = (Page *)bg;
            if (p->cb != NULL)
                p->cb(p, sel_item);
        }
    }
}

//------------信息弹窗相关函数
void OLED_InfoWinInit(PageAddr bg, Option *sel_item) {
    UNUSED_PARAMETER(bg);
    // 窗口高
    if (sel_item->content != NULL)
        p_cur_ui->infoWin.win.h = OLED_GetStrHeight(sel_item->content, INFO_WIN_FONT) + (FONT_MARGIN << 1);
    else
        p_cur_ui->infoWin.win.h = OLED_GetStrHeight(sel_item->text, INFO_WIN_FONT) + (FONT_MARGIN << 1);
    // 窗口上下沿
    p_cur_ui->infoWin.win.u = (OLED_HEIGHT - p_cur_ui->infoWin.win.h) >> 1;
    p_cur_ui->infoWin.win.d = (OLED_HEIGHT + p_cur_ui->infoWin.win.h) >> 1;
    // 窗口y坐标
    p_cur_ui->infoWin.win.y = p_cur_ui->indicator.y;
    p_cur_ui->infoWin.win.y_tgt = p_cur_ui->infoWin.win.u;
    // 背景页面渐变消失
    OLED_UIFadeOutInit(2, p_cur_ui->upara->ani_param[BLUR_ANI]);
}

void OLED_InfoWinShow(Option *sel_item, uint16_t time) {
    // 窗口y坐标运动
    OLED_Animation(&p_cur_ui->infoWin.win.y, &p_cur_ui->infoWin.win.y_tgt, p_cur_ui->upara->ani_param[WIN_ANI], time);
    // 弹窗填充
    OLED_SetPointColor(0);
    OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                     p_cur_ui->indicator.w, p_cur_ui->indicator.h, INFO_WIN_R);
    OLED_SetPointColor(1);
    // 信息弹窗正文
    if (sel_item->content != NULL)
        OLED_WinDrawStr(&w_all, p_cur_ui->infoWin.win.l + FONT_MARGIN, p_cur_ui->infoWin.win.y + FONT_MARGIN,
                        INFO_WIN_FONT, (uint8_t *)sel_item->content);
    else
        OLED_WinDrawStr(&w_all, p_cur_ui->infoWin.win.l + FONT_MARGIN, p_cur_ui->infoWin.win.y + FONT_MARGIN,
                        INFO_WIN_FONT, (uint8_t *)&(sel_item->text[2]));
    // indicator 弹窗边框由指示器绘制
    p_cur_ui->indicator.x_tgt = p_cur_ui->infoWin.win.l;
    p_cur_ui->indicator.y_tgt = (int16_t)p_cur_ui->infoWin.win.u;
    p_cur_ui->indicator.w_tgt = INFO_WIN_W;
    p_cur_ui->indicator.h_tgt = p_cur_ui->infoWin.win.h;
}

void OLED_InfoWinReact(PageAddr bg, Option *sel_item) {
    // if (p_cur_ui->infoWin.win.y == p_cur_ui->infoWin.win.y_tgt && p_cur_ui->infoWin.win.y != (-p_cur_ui->infoWin.win.h)) // 弹窗进场动画结束时
    if (p_cur_ui->infoWin.win.y == p_cur_ui->infoWin.win.y_tgt) // 弹窗进场动画结束时
    {
        InputMsg msg = OLED_MsgQueRead();
        switch (msg) {
        case msg_add:
        case msg_sub:
        case msg_up:
        case msg_down:
            break;
        case msg_click: // 点击确认也会退出弹窗，不过，点击确认是会触发一次回调，点击返回则不会
        case msg_return:
            // p_cur_ui->infoWin.win.y_tgt = -p_cur_ui->infoWin.win.h;  // 弹窗退场
            OLED_UIFadeInInit(p_cur_ui->upara->ani_param[BLUR_ANI]); // 背景页面渐变出现
            p_cur_ui->infoWin.win.state = win_state_close;           // 状态标志，退出弹窗
            break;
        case msg_home:
            break;
        default:
            break;
        }
        if (msg != msg_none && msg != msg_return) // 只有弹窗内有动作，就会实时触发回调
        {
            Page *p = (Page *)bg;
            if (p->cb != NULL)
                p->cb(p, sel_item);
        }
    }
}

//------------确认弹窗相关函数
void OLED_ConWinInit(PageAddr bg, Option *sel_item) {
    UNUSED_PARAMETER(bg);
    // 窗口高
    if (sel_item->content != NULL)
        p_cur_ui->confWin.win.h = OLED_GetStrHeight(sel_item->content, CON_WIN_FONT) + (FONT_MARGIN << 1) + CON_WIN_BTN_H;
    else
        p_cur_ui->confWin.win.h = OLED_GetStrHeight(sel_item->text, CON_WIN_FONT) + (FONT_MARGIN << 1) + CON_WIN_BTN_H;
    // 窗口上下沿
    p_cur_ui->confWin.win.u = (OLED_HEIGHT - p_cur_ui->confWin.win.h) >> 1;
    p_cur_ui->confWin.win.d = (OLED_HEIGHT + p_cur_ui->confWin.win.h) >> 1;
    // 窗口y坐标
    p_cur_ui->confWin.win.y = -p_cur_ui->confWin.win.h;
    p_cur_ui->confWin.win.y_tgt = p_cur_ui->confWin.win.u;
    // 背景页面渐变消失
    OLED_UIFadeOutInit(2, p_cur_ui->upara->ani_param[BLUR_ANI]);
}

void OLED_ConWinShow(Option *sel_item, uint16_t time) {
    // 窗口y坐标运动
    OLED_Animation(&p_cur_ui->confWin.win.y, &p_cur_ui->confWin.win.y_tgt, p_cur_ui->upara->ani_param[WIN_ANI], time);
    // 弹窗填充
    OLED_SetPointColor(0);
    OLED_WinDrawRBox(&w_all, p_cur_ui->confWin.win.l, (int16_t)p_cur_ui->confWin.win.y, CON_WIN_W, p_cur_ui->confWin.win.h, CON_WIN_R);
    OLED_SetPointColor(1);
    // 弹窗外框
    OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->confWin.win.l, (int16_t)p_cur_ui->confWin.win.y, CON_WIN_W, p_cur_ui->confWin.win.h, CON_WIN_R);
    // 信息提示正文
    if (sel_item->content != NULL)
        OLED_WinDrawStr(&w_all, p_cur_ui->confWin.win.l + FONT_MARGIN, p_cur_ui->confWin.win.y + FONT_MARGIN,
                        CON_WIN_FONT, (uint8_t *)sel_item->content);
    else
        OLED_WinDrawStr(&w_all, p_cur_ui->confWin.win.l + FONT_MARGIN, p_cur_ui->confWin.win.y + FONT_MARGIN,
                        CON_WIN_FONT, (uint8_t *)&(sel_item->text[2]));
    // 选项
    OLED_WinDrawStr(&w_all, p_cur_ui->confWin.win.l + 15,
                    (int16_t)p_cur_ui->confWin.win.y + p_cur_ui->confWin.win.h - 11, CON_WIN_FONT, (uint8_t *)"ON");
    OLED_WinDrawStr(&w_all, p_cur_ui->confWin.win.r - 15 - CON_WIN_FONT.Width * 3,
                    (int16_t)p_cur_ui->confWin.win.y + p_cur_ui->confWin.win.h - 11, CON_WIN_FONT, (uint8_t *)"OFF");
    // indicator
    p_cur_ui->indicator.h_tgt = CON_WIN_BTN_H;
    p_cur_ui->indicator.y_tgt = (int16_t)p_cur_ui->confWin.win.d - 14;
    if (sel_item->bit_sel == true) {
        p_cur_ui->indicator.x_tgt = p_cur_ui->confWin.win.l + 15 - CON_WIN_FONT.Width;
        p_cur_ui->indicator.w_tgt = CON_WIN_FONT.Width * 4;
    } else {
        p_cur_ui->indicator.x_tgt = p_cur_ui->confWin.win.r - 15 - CON_WIN_FONT.Width * 4;
        p_cur_ui->indicator.w_tgt = CON_WIN_FONT.Width * 5;
    }
}

void OLED_ConWinReact(PageAddr bg, Option *sel_item) {
    // if (p_cur_ui->confWin.win.y == p_cur_ui->confWin.win.y_tgt && p_cur_ui->confWin.win.y != (-p_cur_ui->confWin.win.h)) // 弹窗进场动画结束
    if (p_cur_ui->confWin.win.y == p_cur_ui->confWin.win.y_tgt) // 弹窗进场动画结束
    {
        InputMsg msg = OLED_MsgQueRead();
        Page *p = (Page *)bg;
        ListPage *lp = (ListPage *)p_cur_ui->current_page;
        switch (msg) {
        case msg_add:
        case msg_sub:
            break;
        case msg_up:
        case msg_down: // 上下键在确认弹窗时用于切换
            sel_item->bit_sel = !(sel_item->bit_sel);
            break;
        case msg_click: // 对于确认弹窗来说，只有按下确认键，才会触发回调
            sel_item->val = sel_item->bit_sel;
            if ((p->cb) != NULL)
                p->cb(p, sel_item);
            // 这儿不用break;因为确认结束，直接退出弹窗
            if (OLED_CheckPageType(p_cur_ui->current_page) == type_list && lp->page_setting == Setting_radio && sel_item->val)
                p_cur_ui->lp_var.radio_click_flag = true; // 标记为true，用于OLED_RadioReact进行轮询判断单选
        case msg_return:
            // p_cur_ui->confWin.win.y_tgt = -p_cur_ui->confWin.win.h;  // 弹窗退场
            OLED_UIFadeInInit(p_cur_ui->upara->ani_param[BLUR_ANI]); // 背景页面渐变出现
            p_cur_ui->confWin.win.state = win_state_close;           // 标志退出弹窗状态
            break;
        case msg_home:
            return;
        default:
            break;
        }
    }
}

//------------微调数值弹窗相关函数
void OLED_SpinWinInit(PageAddr bg, Option *sel_item) {
    UNUSED_PARAMETER(bg);
    // 窗口y坐标
    p_cur_ui->spinWin.win.y = SPIN_WIN_Y_TGT;
    p_cur_ui->spinWin.win.y_tgt = p_cur_ui->spinWin.win.u;
    // 字符串
    switch (sel_item->decimalNum) {
    case DecimalNum_0:
        sprintf(p_cur_ui->spinWin.range_str, "%d~%d", sel_item->min, sel_item->max);
        break;
    case DecimalNum_1:
        sprintf(p_cur_ui->spinWin.range_str, "%g~%g", sel_item->min / 10.0f, sel_item->max / 10.0f);
        break;
    case DecimalNum_2:
        sprintf(p_cur_ui->spinWin.range_str, "%g~%g", sel_item->min / 100.0f, sel_item->max / 100.0f);
        break;
    case DecimalNum_3:
        sprintf(p_cur_ui->spinWin.range_str, "%g~%g", sel_item->min / 1000.0f, sel_item->max / 1000.0f);
        break;
    default:
        break;
    }
    p_cur_ui->spinWin.title_w = OLED_GetStrWidth((char *)&(sel_item->text[2]), WIN_FONT);
    p_cur_ui->spinWin.range_str_w = OLED_GetStrWidth((char *)&(p_cur_ui->spinWin.range_str), WIN_FONT);
    // 设置
    sel_item->bit_sel = false;
    // 背景页面渐变消失
    OLED_UIFadeOutInit(2, p_cur_ui->upara->ani_param[BLUR_ANI]);
}

void OLED_SpinWinShow(Option *sel_item, uint16_t time) {
    // 窗口y坐标运动
    OLED_Animation(&p_cur_ui->spinWin.win.y, &p_cur_ui->spinWin.win.y_tgt, p_cur_ui->upara->ani_param[WIN_ANI], time);
    // 弹窗填充
    OLED_SetPointColor(0);
    OLED_WinDrawRBox(&w_all, p_cur_ui->spinWin.win.l, (int16_t)p_cur_ui->spinWin.win.y,
                     SPIN_WIN_W, SPIN_WIN_H, 2);
    // 弹窗外框
    OLED_SetPointColor(1);
    OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->spinWin.win.l, (int16_t)p_cur_ui->spinWin.win.y,
                          SPIN_WIN_W, SPIN_WIN_H, 2);
    // 提示文本跳过“% ”
    OLED_WinDrawStr(&w_all, (OLED_WIDTH - p_cur_ui->spinWin.title_w) / 2,
                    (int16_t)p_cur_ui->spinWin.win.y + FONT_MARGIN, WIN_FONT, (uint8_t *)&(sel_item->text[2]));
    // value range
    OLED_WinDrawStr(&w_all, (OLED_WIDTH - p_cur_ui->spinWin.range_str_w) / 2,
                    (int16_t)p_cur_ui->spinWin.win.y + 18, SPIN_WIN_FONT, (uint8_t *)p_cur_ui->spinWin.range_str);
    // decimal point
    switch (sel_item->decimalNum) {
    case DecimalNum_0:
        break;
    case DecimalNum_1:
        OLED_WinDrawASCII(&w_all, OLED_MIDDLE_H + 24, (int16_t)p_cur_ui->spinWin.win.y + 29, SPIN_WIN_NUM_FONT, '.');
        break;
    case DecimalNum_2:
        OLED_WinDrawASCII(&w_all, OLED_MIDDLE_H + 15, (int16_t)p_cur_ui->spinWin.win.y + 29, SPIN_WIN_NUM_FONT, '.');
        break;
    case DecimalNum_3:
        OLED_WinDrawASCII(&w_all, OLED_MIDDLE_H + 6, (int16_t)p_cur_ui->spinWin.win.y + 29, SPIN_WIN_NUM_FONT, '.');
        break;
    default:
        break;
    }
    // num buffer
    char numBuff[12];
    sprintf(numBuff, "%+08d", sel_item->val);
    // show numDigit
    uint8_t num_x = (OLED_WIDTH - SPIN_WIN_NUM_W) / 2;
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WinDrawASCII(&w_all, num_x, (int16_t)p_cur_ui->spinWin.win.y + 29, SPIN_WIN_NUM_FONT, numBuff[i]);
        num_x += 9;
    }
    // indicator
    p_cur_ui->indicator.w_tgt = 9;
    p_cur_ui->indicator.x_tgt = (OLED_WIDTH - SPIN_WIN_NUM_W) / 2 + (7 - sel_item->step) * 9 - 1;
    if (sel_item->bit_sel) {
        p_cur_ui->indicator.y_tgt = p_cur_ui->spinWin.win.u + 27;
        p_cur_ui->indicator.h_tgt = 14;
    } else {
        p_cur_ui->indicator.y_tgt = p_cur_ui->spinWin.win.u + 39;
        p_cur_ui->indicator.h_tgt = 3;
    }
}

void OLED_SpinWinReact(PageAddr bg, Option *sel_item) {
    int32_t delta_val = 0;
    bool reverse = false;
    // if (p_cur_ui->spinWin.win.y == p_cur_ui->spinWin.win.y_tgt && p_cur_ui->spinWin.win.y != SPIN_WIN_Y_TGT) // 弹窗进场动画结束时
    if (p_cur_ui->spinWin.win.y == p_cur_ui->spinWin.win.y_tgt) // 弹窗进场动画结束时
    {
        InputMsg msg = OLED_MsgQueRead();
        switch (msg) {
        case msg_up:
            if (!sel_item->bit_sel && sel_item->step < 7)
                sel_item->step++;
            break;
        case msg_down:
            if (!sel_item->bit_sel && sel_item->step > 0)
                sel_item->step--;
            break;
        case msg_add:
            if (sel_item->bit_sel) {
                delta_val = 1;
                for (uint8_t i = 0; i < sel_item->step; i++)
                    delta_val *= 10;
            }
            break;
        case msg_sub:
            if (sel_item->bit_sel) {
                delta_val = -1;
                for (uint8_t i = 0; i < sel_item->step; i++)
                    delta_val *= 10;
            }
            break;
        case msg_click:
            if (sel_item->step == 7)
                reverse = true;
            else
                sel_item->bit_sel = !sel_item->bit_sel;
            break;
        case msg_return:
            // p_cur_ui->spinWin.win.y_tgt = SPIN_WIN_Y_TGT;            // 弹窗退场
            OLED_UIFadeInInit(p_cur_ui->upara->ani_param[BLUR_ANI]); // 背景页面渐变出现
            p_cur_ui->spinWin.win.state = win_state_close;           // 状态标志，退出弹窗
            break;
        case msg_home:
            return;
        default:
            break;
        }
        if (msg != msg_none && msg != msg_return) // 只有弹窗内有动作，就会实时触发回调
        {
            Page *p = (Page *)bg;
            if (p->cb != NULL)
                p->cb(p, sel_item);
        }
    }
    int32_t val_last = sel_item->val;
    if (delta_val != 0) {
        sel_item->val += delta_val;
    }
    if (reverse)
        sel_item->val = -sel_item->val;
    if (sel_item->val < sel_item->min || sel_item->val > sel_item->max)
        sel_item->val = val_last;
}
