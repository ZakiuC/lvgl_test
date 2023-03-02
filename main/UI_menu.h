/*
 * @Author       : zakiuc
 * @Date         : 2023-02-23 13:20:56
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 10:35:08
 * @FilePath     : \main\UI_menu.h
 * @Description  : 
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#ifndef _UI_MENU_H_
#define _UI_MENU_H_
#include "lvgl.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "menu_list.h"
#include "user_config.h"

typedef struct
{
    // 屏幕
    lv_obj_t* Screen;
    // 导航条
    lv_obj_t* tabbar;

    // 菜单头节点 
    Menu_t menu_list; 

    // 模式号
    u8 index;
}lv_ui_t;


#define MENU_MOVE_TIME 200
#define MENU_MOVE_ON 1
#define MENU_MOVE_BACK 0
#define MODE_ICON_DEFAULT_POS 70
#define MODE_LABEL_DEFAULT_POS 20

// 导入图片
LV_IMG_DECLARE(_DeepMode_100x100);
LV_IMG_DECLARE(_DailyMode_100x100);
LV_IMG_DECLARE(_MoreMode_100x100);

// 切换菜单(单步)
u8 menu_move_step(lv_ui_t* ui, u8 direction);
// 切换菜单
u8 menu_move(lv_ui_t* ui, u8 target);

// 初始化菜单ui
void init_menu_ui(lv_ui_t* my_ui);
#endif // _UI_MENU_H_