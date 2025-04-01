#include "WouoUI_page.h"
#include "WouoUI.h"

//**********************通用页面处理方法
/**
 * @brief 通用的页面返回上一个页面函数
 *
 * @param page_addr 当前页面地址
 */
void OLED_PageReturn(PageAddr page_addr) {
    Page *p = (Page *)page_addr;
    p_cur_ui->state = ui_page_out;    // 将状态机置为页面初始化状态
    p_cur_ui->in_page = p->last_page; // 将UI页面设置为上一级页面
    OLED_UIFadeOutInit(4, p_cur_ui->upara->ani_param[FADE_ANI]);
    // 从主页面返回
    if (p == p_cur_ui->home_page) {
        p_cur_ui->indicator.x = 0;
        p_cur_ui->indicator.y = 0;
        p_cur_ui->indicator.w = OLED_WIDTH;
        p_cur_ui->indicator.h = OLED_HEIGHT;
    }
}

/**
 * @brief
 *
 * @param page_addr 最基本的页面成员初始化，并加入到页面队列中
 * @param call_back 页面成员点击回调函数
 * @attention 没有进行页面类型初始化，因为这个由各自初始化函数执行
 */
void OLED_PageInit(PageAddr page_addr, CallBackFunc call_back) {
    Page *p = (Page *)page_addr;
    if (p_cur_ui->home_page == NULL) // 第一个页面初始化
    {
        p_cur_ui->home_page = page_addr;    // 将其初始化为主页面
        p_cur_ui->current_page = page_addr; // 同时作为当前页面
    }
    p->last_page = p_cur_ui->home_page;
    // 页面初始化时没有上一级页面,只有在页面跳转时才确定页面的上下级关系,默认将主页面(第一个初始化的页面)作为上一级页面
    p->cb = call_back;
}

//**********************每个以Page为基类的类都有对应的AnimInit、show、React方法
//--------Title相关函数
void OLED_TitlePageEnterInit(PageAddr page_addr, uint16_t time) {
    p_cur_ui->tp_var.icon_x = 0;
    p_cur_ui->tp_var.icon_x_tgt = TILE_ICON_S;
    p_cur_ui->tp_var.icon_y = -TILE_ICON_H / 2;
    p_cur_ui->tp_var.icon_y_tgt = TILE_ICON_U;
    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
    p_cur_ui->tp_var.bar_x_tgt = 0;
    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
    p_cur_ui->tp_var.title_y_tgt = OLED_HEIGHT - TILE_BAR_D - TILE_B_TITLE_FONT.Height;
}

void OLED_TitlePageShow(PageAddr page_addr, uint16_t time) {
    TitlePage *tp = (TitlePage *)page_addr;
    int16_t temp = 0; // 用于存放临时的icon的x坐标
    // 计算动画参数
    OLED_Animation(&p_cur_ui->tp_var.icon_x, &p_cur_ui->tp_var.icon_x_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);   // 图标x坐标
    OLED_Animation(&p_cur_ui->tp_var.icon_y, &p_cur_ui->tp_var.icon_y_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);   // 图标y坐标
    OLED_Animation(&p_cur_ui->tp_var.bar_x, &p_cur_ui->tp_var.bar_x_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);     // 装饰条x坐标
    OLED_Animation(&p_cur_ui->tp_var.title_y, &p_cur_ui->tp_var.title_y_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time); // 文字y坐标
    // 绘制title
    String show_str = tp->option_array[tp->select_item].text;
    OLED_WinDrawStr(&w_all, OLED_MIDDLE_H - OLED_GetStrWidth(&(show_str[2]), TILE_B_TITLE_FONT) / 2,
                    p_cur_ui->tp_var.title_y, TILE_B_TITLE_FONT, (uint8_t *)&(show_str[2]));
    // 绘制装饰条
    OLED_WinDrawRBox(&w_all, p_cur_ui->tp_var.bar_x, OLED_HEIGHT - TILE_BAR_D - TILE_B_TITLE_FONT.Height,
                     TILE_BAR_W, TILE_BAR_H, 0);
    // 绘制图标
    if (!p_cur_ui->init_finish_flag) // 过度动画尚未完成
    {
        for (uint8_t i = 0; i < tp->item_num; i++) {
            if (p_cur_ui->upara->ufd_param[TILE_UFD]) // 从第一个选项开始展开，最终保持选中在中间
                temp = ((OLED_WIDTH - TILE_ICON_W) >> 1) + i * (int16_t)p_cur_ui->tp_var.icon_x - TILE_ICON_S * tp->select_item;
            else // 保证选中的选项在中间，向两侧展开
                temp = ((OLED_WIDTH - TILE_ICON_W) >> 1) + (i - tp->select_item) * (int16_t)p_cur_ui->tp_var.icon_x;
            OLED_WinDrawBMP(&w_all, temp, (int16_t)(p_cur_ui->tp_var.icon_y), TILE_ICON_W, TILE_ICON_H, tp->icon_array[i], 1);
        }
        if (p_cur_ui->tp_var.icon_x == p_cur_ui->tp_var.icon_x_tgt) {
            p_cur_ui->init_finish_flag = true; // 设置过度动画已经结束
            p_cur_ui->tp_var.icon_x = p_cur_ui->tp_var.icon_x_tgt = -1 * tp->select_item * TILE_ICON_S;
        }
    } else {
        for (uint8_t i = 0; i < tp->item_num; i++) // 过度动画完成后一般选择时的切换动画
            OLED_WinDrawBMP(&w_all, (OLED_WIDTH - TILE_ICON_W) / 2 + (int16_t)(p_cur_ui->tp_var.icon_x) + i * TILE_ICON_S,
                            TILE_ICON_U, TILE_ICON_W, TILE_ICON_H, tp->icon_array[i], 1);
    }
    // indicator
    p_cur_ui->indicator.x_tgt = (OLED_WIDTH - TILE_ICON_IND_W) / 2;
    p_cur_ui->indicator.y_tgt = TILE_ICON_U - (TILE_ICON_IND_W - TILE_ICON_W) / 2;
    p_cur_ui->indicator.w_tgt = TILE_ICON_IND_W;
    p_cur_ui->indicator.h_tgt = TILE_ICON_IND_H;
}

void OLED_TitlePageReact(PageAddr page_addr, uint16_t time) {
    TitlePage *tp = (TitlePage *)page_addr;
    String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead(); // 空时读出msg_none
    switch (msg) {
    case msg_add:
    case msg_sub:
        break;
    case msg_up:
        if (p_cur_ui->init_finish_flag) // 已经完成过渡动画了
        {
            if (tp->select_item > 0) { // 不是第一个选项
                tp->select_item--;
                p_cur_ui->tp_var.icon_x_tgt += TILE_ICON_S;
                p_cur_ui->indicator.x += (TILE_ICON_W >> 1);
                p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                p_cur_ui->tp_var.title_y = OLED_HEIGHT;
            } else {                                        // 是第一个选项
                if (p_cur_ui->upara->loop_param[TILE_LOOP]) // 开启循环的话
                {
                    tp->select_item = tp->item_num - 1;
                    p_cur_ui->tp_var.icon_x_tgt = -1 * TILE_ICON_S * (tp->item_num - 1);
                    p_cur_ui->indicator.x -= (TILE_ICON_W >> 1);
                    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
                }
            }
        }
        break;
    case msg_down:
        if (p_cur_ui->init_finish_flag) // 已经完成过渡动画了
        {
            if (tp->select_item < (tp->item_num - 1)) { // 不是最后一个选项
                tp->select_item++;
                p_cur_ui->tp_var.icon_x_tgt -= TILE_ICON_S;
                p_cur_ui->indicator.x -= (TILE_ICON_W >> 1);
                p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                p_cur_ui->tp_var.title_y = OLED_HEIGHT;
            } else { // 是最后一个选项
                if (p_cur_ui->upara->loop_param[TILE_LOOP]) {
                    tp->select_item = 0;
                    p_cur_ui->tp_var.icon_x_tgt = 0;
                    p_cur_ui->indicator.x += (TILE_ICON_W >> 1);
                    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
                }
            }
        }
        break;
    case msg_click:
        selcet_string = tp->option_array[tp->select_item].text;
        if (selcet_string[0] == '@') { // 二值选框
            if (tp->option_array[tp->select_item].step)
                tp->option_array[tp->select_item].val = tp->option_array[tp->select_item].val; // 将对应值取反
        } else if (selcet_string[0] == '~') {                                                  // 滑动数值弹窗的话，进入数值弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            // 当数值弹窗step设为0为只读是，进入数值弹窗前会调用一次回调，以便外界更改val值
            p_cur_ui->slideValWin.win.state = win_state_init; // 进入弹窗初始化
            if ((tp->option_array[tp->select_item].step == 0) && (tp->page.cb != NULL))
                tp->page.cb(&(tp->page), &(tp->option_array[tp->select_item]));
        } else if (selcet_string[0] == '%') { // 微调数值弹窗
            p_cur_ui->spinWin.win.state = win_state_init;
        } else if (selcet_string[0] == '!') { // 信息弹窗
            p_cur_ui->infoWin.win.state = win_state_init;
        } else if (selcet_string[0] == '#') { // 确认弹窗
            tp->option_array[tp->select_item].bit_sel = tp->option_array[tp->select_item].val;
            p_cur_ui->confWin.win.state = win_state_init; // 确认弹窗内只有确认弹窗内的click才会触发回调
        } else if (tp->page.cb != NULL)
            tp->page.cb(&(tp->page), &(tp->option_array[tp->select_item]));
        // 当前页面id和确认选中项的指针传入
        break;
    case msg_return:
        OLED_PageReturn(page_addr);
        break;
    case msg_home:
        break;
    default:
        break;
    }
}

void OLED_TitlePageInit(
    TitlePage *title_page,  // 磁贴页面对象
    uint8_t item_num,       // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    Icon *icon_array,       // 整个页面的icon数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
{
    // 缺少选项个数检查，Icon个数检查，
    title_page->page.page_type = type_title;
    OLED_PageInit((PageAddr)title_page, call_back);
    title_page->page.init = OLED_TitlePageEnterInit;
    title_page->page.show = OLED_TitlePageShow;
    title_page->page.react = OLED_TitlePageReact; // 关联处理函数(方法)
    title_page->select_item = 0;
    title_page->item_num = item_num;
    title_page->option_array = option_array;
    title_page->icon_array = icon_array;
    for (uint8_t i = 0; i < title_page->item_num; i++)
        title_page->option_array[i].order = i; // 选项序号标号
}

//-------List页面相关函数
// 绘制列表项文字和对应的行末数字或选框
void OLED_ListDrawText_CheckBox(int16_t start_y, Option *item) {
    // 绘制文字
    OLED_WinDrawStr(&w_all, LIST_TEXT_L_S, start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)(item->text));
    // 绘制表尾
    char val_buff[16];
    switch (item->text[0]) {
    case '~': // 滑动数值弹窗，画行结尾的数值
        ui_itoa_str(item->val, val_buff);
        OLED_WinDrawStr(&w_all, OLED_WIDTH - LIST_TEXT_R_S - OLED_GetStrWidth(val_buff, LIST_TEXT_FONT),
                        start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)val_buff);
        break;
    case '=': // 数值弹窗，画行结尾的数值
        ui_ftoa_str(item->val, item->decimalNum, val_buff);
        OLED_WinDrawStr(&w_all, OLED_WIDTH - LIST_TEXT_R_S - OLED_GetStrWidth(val_buff, LIST_TEXT_FONT),
                        start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)val_buff);
        break;
    case '%':
        ui_ftoa_str(item->val, item->decimalNum, val_buff);
        OLED_WinDrawStr(&w_all, OLED_WIDTH - LIST_TEXT_R_S - OLED_GetStrWidth(val_buff, LIST_TEXT_FONT),
                        start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)val_buff);
        break;
    case '@': // 如果是二值选框
    case '#': // 如果是二值确认弹窗
        OLED_WinDrawRBoxEmpty(&w_all, OLED_WIDTH - CHECK_BOX_R_S - CHECK_BOX_F_W,
                              start_y + CHECK_BOX_U_S, CHECK_BOX_F_W, CHECK_BOX_F_H, 2);
        if (item->val != 0) // 非0即是选中，画框内的点
            OLED_WinDrawRBox(&w_all, OLED_WIDTH - CHECK_BOX_R_S - CHECK_BOX_F_W + CHECK_BOX_D_S,
                             start_y + CHECK_BOX_U_S + CHECK_BOX_D_S, CHECK_BOX_D_W, CHECK_BOX_D_H, 1);
        break;
    default:
        break;
    }
}

void OLED_ListPageEnterInit(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    p_cur_ui->lp_var.y = 0;
    p_cur_ui->lp_var.y_tgt = LIST_LINE_H;
    p_cur_ui->scrollBar.display = true;
}

void OLED_ListPageShow(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    static int16_t temp = 0; // 用于临时存放列表每一项的y坐标
    // 计算动画过渡值
    OLED_Animation(&(p_cur_ui->lp_var.y), &(p_cur_ui->lp_var.y_tgt), p_cur_ui->upara->ani_param[LIST_ANI], time);
    if (p_cur_ui->lp_var.slip_flag && p_cur_ui->indicator.y == lp->ind_y_tgt)
        p_cur_ui->lp_var.slip_flag = false;
    // 该标志位是为了防止 切换动画导致的box_y移动对过度动画造成影响
    if (!p_cur_ui->init_finish_flag) // 过度动画
    {
        for (uint8_t i = 0; i < (lp->item_num); i++) {
            if (p_cur_ui->upara->ufd_param[LIST_UFD]) // 从头展开
                temp = i * (p_cur_ui->lp_var.y) - LIST_LINE_H * (lp->select_item) + lp->ind_y_tgt;
            else // 选中项展开
                temp = (i - (lp->select_item)) * (p_cur_ui->lp_var.y) + lp->ind_y_tgt;
            OLED_ListDrawText_CheckBox(temp, &(lp->option_array[i]));
        }
        if (p_cur_ui->lp_var.y == p_cur_ui->lp_var.y_tgt) // 动画抵达目标位置
        {
            p_cur_ui->init_finish_flag = true; // 过度动画完成
            p_cur_ui->lp_var.y = p_cur_ui->lp_var.y_tgt = -LIST_LINE_H * (lp->select_item) + lp->ind_y_tgt;
            // 第一个选项到光标(盒子)所在的距离确定下来
        }
    } else // 过度后，一般上下切换选项时的动画，但目标参数不在这儿设置
        for (uint8_t i = 0; i < (lp->item_num); i++) {
            temp = LIST_LINE_H * i + (p_cur_ui->lp_var.y);
            OLED_ListDrawText_CheckBox(temp, &(lp->option_array[i]));
        }

    // indicator
    p_cur_ui->indicator.x_tgt = 0;
    p_cur_ui->indicator.y_tgt = lp->ind_y_tgt;
    p_cur_ui->indicator.w_tgt = OLED_GetStrWidth(lp->option_array[lp->select_item].text, LIST_TEXT_FONT) + LIST_TEXT_L_S * 2;
    p_cur_ui->indicator.h_tgt = LIST_LINE_H;
    // scrollBar
    p_cur_ui->scrollBar.y_tgt = (int16_t)(lp->select_item * OLED_HEIGHT / (lp->item_num - 1)) + 1;
}

void OLED_ListPageReact(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead();
    bool select_val;
    switch (msg) {
    case msg_add:
    case msg_sub:
        break;
    case msg_up:                                                         // 上一个
        if (p_cur_ui->init_finish_flag && !(p_cur_ui->lp_var.slip_flag)) // 初始化动画已完成
        {
            if (lp->select_item == 0) // 选中第一个的话
            {
                if (p_cur_ui->upara->loop_param[LIST_LOOP]) // 同时loop参数开的话
                {
                    p_cur_ui->lp_var.slip_flag = true;
                    lp->select_item = lp->item_num - 1; // 选中最后一个
                    if (lp->item_num > lp->line_n)      // 数目超出一页的最大数目
                    {                                   // 更改box到最底
                        lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        // 同时更改文字到最底
                        p_cur_ui->lp_var.y_tgt = OLED_HEIGHT - (lp->item_num) * LIST_LINE_H;
                    } else // 没有超出数目则是到最后一个
                        lp->ind_y_tgt = (lp->item_num - 1) * LIST_LINE_H;
                }
            } else // 没有选中第一个
            {
                lp->select_item -= 1;                                            // 选中减1
                if (lp->select_item < -((p_cur_ui->lp_var.y_tgt) / LIST_LINE_H)) // 光标盒子到页面顶了????
                {
                    if (!(OLED_HEIGHT % LIST_LINE_H))          // 上面剩余完整的行
                        p_cur_ui->lp_var.y_tgt += LIST_LINE_H; // 文字下移
                    else                                       // 上面的行不完整(LIST_LINE_H该项用于页面和行高非整除时)
                    {
                        if (lp->ind_y_tgt == OLED_HEIGHT - LIST_LINE_H * lp->line_n) { // 文字往下走一行，且光标盒子置于0处，把上面的非整行去掉
                            p_cur_ui->lp_var.y_tgt += (lp->line_n + 1) * LIST_LINE_H - OLED_HEIGHT;
                            lp->ind_y_tgt = 0;
                        } else if (lp->ind_y_tgt == LIST_LINE_H)
                            lp->ind_y_tgt = 0; // 上面整行直接移动光标盒子
                        else
                            p_cur_ui->lp_var.y_tgt += LIST_LINE_H; // 顶页整行，文字直接往下走
                    }
                } else                            // 光标盒子没到页面顶
                    lp->ind_y_tgt -= LIST_LINE_H; // 直接光标盒子往上走即可。
            }
        }
        break;
    case msg_down:                                                       // 下一个
        if (p_cur_ui->init_finish_flag && !(p_cur_ui->lp_var.slip_flag)) // 初始化动作已完成
        {
            if (lp->select_item == (lp->item_num) - 1) // 到最后一个选项了
            {
                if (p_cur_ui->upara->loop_param[LIST_LOOP]) // loop开关开
                {                                           // 全部回到顶部
                    p_cur_ui->lp_var.slip_flag = true;
                    lp->select_item = 0;
                    p_cur_ui->lp_var.y_tgt = 0;
                    lp->ind_y_tgt = 0;
                    p_cur_ui->lp_var.y_tgt = 0;
                }
            } else // 不是最后一个选项
            {
                lp->select_item += 1;
                if ((lp->select_item + 1) > ((lp->line_n) - (p_cur_ui->lp_var.y_tgt) / LIST_LINE_H)) { // 光标到页面底
                    if (!(OLED_HEIGHT % LIST_LINE_H))
                        p_cur_ui->lp_var.y_tgt -= LIST_LINE_H;
                    else // 非整行的情况
                    {
                        if (lp->ind_y_tgt == LIST_LINE_H * (lp->line_n - 1)) {
                            p_cur_ui->lp_var.y_tgt -= (lp->line_n + 1) * LIST_LINE_H - OLED_HEIGHT;
                            // 取出要向下移动的距离，对y_tgt来说就是向上移动的距离
                            lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        } else if (lp->ind_y_tgt == OLED_HEIGHT - LIST_LINE_H * 2)
                            // 这种情况什么时候会出现呢嗯?
                            lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        else
                            p_cur_ui->lp_var.y_tgt -= LIST_LINE_H;
                    }
                } else
                    lp->ind_y_tgt += LIST_LINE_H;
            }
        }
        break;
    case msg_click:
        selcet_string = lp->option_array[lp->select_item].text;
        select_val = lp->option_array[lp->select_item].val;
        if (selcet_string[0] == '@') { // 二值选框
            if (lp->option_array[lp->select_item].step)
                lp->option_array[lp->select_item].val = !select_val; // 将对应值取反
            if (lp->page_setting == Setting_radio)
                p_cur_ui->lp_var.radio_click_flag = true; // 标记为1，用于OLED_RadioReact进行轮询判断单选
        } else if (selcet_string[0] == '~') {             // 滑动数值弹窗的话，进入数值弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            // 当数值弹窗step设为0为只读是，进入数值弹窗前会调用一次回调，以便外界更改val值
            p_cur_ui->slideValWin.win.state = win_state_init; // 进入弹窗初始化
            if ((lp->option_array[lp->select_item].step == 0) && (lp->page.cb != NULL))
                lp->page.cb(&(lp->page), &(lp->option_array[lp->select_item]));
        } else if (selcet_string[0] == '%') {
            p_cur_ui->spinWin.win.state = win_state_init; // 进入弹窗初始化
        } else if (selcet_string[0] == '!') {             // 信息弹窗的话，进入信息弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            p_cur_ui->infoWin.win.state = win_state_init; // 进入弹窗初始化
        } else if (selcet_string[0] == '#') {             // 进入确认弹窗初始化
            lp->option_array[lp->select_item].bit_sel = lp->option_array[lp->select_item].val;
            p_cur_ui->confWin.win.state = win_state_init; // 确认弹窗内只有确认弹窗内的click才会触发回调
        } else if (lp->page.cb != NULL)
            lp->page.cb(&(lp->page), &(lp->option_array[lp->select_item]));
        break;
    case msg_return:
        OLED_PageReturn((PageAddr)lp);
        break;
    case msg_home:
        return;
    default:
        break;
    }

    // 单选框页面
    if (lp->page_setting == Setting_radio) {
        if (p_cur_ui->lp_var.radio_click_flag) {
            for (uint8_t i = 0; i < lp->item_num; i++) {
                if (lp->option_array[i].text[0] == '@' || lp->option_array[i].text[0] == '#') {
                    if (i != lp->select_item)
                        lp->option_array[i].val = false;
                    else
                        lp->option_array[i].val = true; // 确保单选
                }
            }
            p_cur_ui->lp_var.radio_click_flag = false;
        }
    }
}

void OLED_ListPageInit(
    ListPage *lp,             // 列表页面对象
    uint8_t item_num,         // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,     // 整个页面的选项数组(数组大小需与item_num一致)
    PageSetting page_setting, // 页面设置
    CallBackFunc call_back    // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
) {
    // 缺少选项个数检查，检查，
    lp->page.page_type = type_list;
    lp->page.init = OLED_ListPageEnterInit;
    lp->page.show = OLED_ListPageShow;
    lp->page.react = OLED_ListPageReact;
    OLED_PageInit((PageAddr)lp, call_back);
    lp->page_setting = page_setting;
    lp->item_num = item_num;
    lp->select_item = 0;
    lp->option_array = option_array;
    lp->line_n = OLED_HEIGHT / LIST_LINE_H;
    for (uint8_t i = 0; i < lp->item_num; i++)
        lp->option_array[i].order = i; // 选项序号标号
}

// ------终端波形页面
#if PAGE_WAVE_ENABLE

void OLED_WavePageAnimInit(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    OLED_ListPageEnterInit(wp, time);
    p_cur_ui->wt_var.p_rear = 0;
    p_cur_ui->wt_var.p_head = 0; // 重新进入页面时，会重新开始显示波形
    p_cur_ui->wt_var.text_bg_r = 0;
    p_cur_ui->wt_var.text_bg_r_tgt = WAVE_TEXT_BG_W;
}

void OLED_WavePageShow(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    int16_t temp; // 临时变量用于存放波形的点，暂时显示
    String temp_str = NULL;
    OLED_ListPageShow(wp, time);
    OLED_WinDrawRBoxEmpty(&w_all, WAVE_BOX_L_S, 0, WAVE_BOX_W, WAVE_BOX_H, 0);
    OLED_WinDrawRBoxEmpty(&w_all, WAVE_BOX_L_S + 1, 1, WAVE_BOX_W - 2, WAVE_BOX_H - 2, 0);

    // 数据入队列
    if (wp->option_array[wp->select_item].step == 1) // 有数据更新，才将数据入队
    {
        p_cur_ui->wt_var.p_rear++;
        p_cur_ui->wt_var.p_rear %= (WAVE_W - 1);
        p_cur_ui->wt_var.data_que[p_cur_ui->wt_var.p_rear] = wp->option_array[wp->select_item].val; // 放入数据
        wp->option_array[wp->select_item].step = 0;                                                 // 将数据更新的标志位置0
    }
    if ((p_cur_ui->wt_var.p_rear + 1) % (WAVE_W - 1) == p_cur_ui->wt_var.p_head) // 队列满了
    {
        p_cur_ui->wt_var.p_head++;
        p_cur_ui->wt_var.p_head %= (WAVE_W - 1); // 环状前进
    }
    p_cur_ui->wt_var.p_font = p_cur_ui->wt_var.p_head;                                                   // 获取起始地址
    if (p_cur_ui->indicator.y == p_cur_ui->lp_var.y_tgt && p_cur_ui->lp_var.y == p_cur_ui->lp_var.y_tgt) // 完成过度动画
    {
        for (uint8_t i = 1; i < WAVE_W - 1; i++) {
            if (p_cur_ui->wt_var.p_rear != p_cur_ui->wt_var.p_font) // 队列非空
            {
                temp = p_cur_ui->wt_var.data_que[p_cur_ui->wt_var.p_font];
                p_cur_ui->wt_var.p_font++;
                p_cur_ui->wt_var.p_font %= (WAVE_W - 1); // 出队指针前移
                // 画点或者差值画斜线函数
                OLED_WinDrawPoint(&w_all, WAVE_L + i,
                                  WAVE_BOX_H - 2 - (temp - wp->option_array[wp->select_item].min) * (WAVE_BOX_H - 4) / (wp->option_array[wp->select_item].max - wp->option_array[wp->select_item].min));
                // 因为0会占掉一个像素点，所以波形没有完全对称。
            } else
                break;
        }
        // 绘制文字
        temp_str = ui_itoa(wp->option_array[wp->select_item].val);
        OLED_WinDrawStr(&w_all, WAVE_TEXT_BG_L_S + (WAVE_TEXT_BG_W - OLED_GetStrWidth(temp_str, WAVE_FONT)) / 2,
                        OLED_HEIGHT - 28, WAVE_FONT, (uint8_t *)temp_str);
        // 绘制选框
        OLED_SetPointColor(2); // 反色选框
        // 注意这个动态宽度在过度动画完成后才执行，且从0到宽度，不是从中间开始的
        OLED_Animation(&p_cur_ui->wt_var.text_bg_r, &p_cur_ui->wt_var.text_bg_r_tgt, p_cur_ui->upara->ani_param[TAG_ANI], time);
        OLED_WinDrawRBox(&w_all, WAVE_TEXT_BG_L_S, OLED_HEIGHT - WAVE_TEXT_BG_H, p_cur_ui->wt_var.text_bg_r, WAVE_TEXT_BG_H, 0);
        OLED_SetPointColor(1); // 恢复实色
    }
}

void OLED_WavePageReact(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    OLED_ListPageReact(wp, time);
    if (p_cur_ui->wt_var.change_flag) {
        p_cur_ui->wt_var.p_rear = 0;
        p_cur_ui->wt_var.p_head = 0; // 切换选项时，重新开始显示波形
        p_cur_ui->wt_var.text_bg_r = 0;
        p_cur_ui->wt_var.text_bg_r_tgt = WAVE_TEXT_BG_W; // 切换选项时唤醒tag动画
        p_cur_ui->wt_var.change_flag = false;
    }
}

void OLED_WavePageInit(
    WavePage *wp,           // 波形页面对象
    uint8_t item_num,       // 选项个数，需与option_array数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为 self_id 和 选中项option指针
{
    OLED_ListPageInit(wp, item_num, option_array, Setting_none, call_back); // 先初始化为list
    wp->page.page_type = type_wave;                                         // 指定为波形页面
    wp->page.init = OLED_WavePageAnimInit;
    wp->page.show = OLED_WavePageShow;
    wp->page.react = OLED_WavePageReact;
}

// 更新波形选项的显示值(后期会加上断言)，以防止输入NULL
void OLED_UIWaveUpdateVal(Option *op, int16_t val) {
    // 这里可以添加断言
    op->step = 1; // wave中的step用于指示是否更新这个波形的数值
    op->val = val;
}

#endif
