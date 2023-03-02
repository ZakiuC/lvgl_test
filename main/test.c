/*
 * @Author       : zakiuc
 * @Date         : 2023-02-21 19:27:38
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 13:19:03
 * @FilePath     : \main\test.c
 * @Description  :
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved.
 */
#include "test.h"
#include "ESP_LOG.h"

#include "controller.h"
#include "UI_menu.h"
#define ESP_TAG "TEST_LOG"

// 控制器
static Controller_t Con;
// ui
static lv_ui_t ui;



/**
 * @brief    旋钮事件回调
 * @param    (u8) cmd : 旋钮命令
 * @return   (void)
 */
void knob_cb(u8 cmd, s32 step)
{
    // 动画执行中不切换
    if(lv_anim_count_running() == 0)
    {
        switch (cmd)
        {
            case RIGHT:
                    menu_move_step(&ui, MENU_MOVE_ON);
                break;

            case LEFT:
                    menu_move_step(&ui, MENU_MOVE_BACK);
                break;
        }
    }

}

/**
 * @brief    按键事件回调
 * @param    (u8) index :
 * @param    (u8) cmd : 按键命令
 * @return   (void)
 */
void key_cb(u8 index, u8 cmd)
{
    // 动画执行中不切换
    if(lv_anim_count_running() == 0)
    {
        switch (cmd)
        {
            case CLICK:
                ESP_LOGI("TT", "单击");
                break;
            case PRESSED:
                ESP_LOGI("TT", "长按");
                break;
            case DOUBLECLICKED:
                ESP_LOGI("TT", "双击");
                menu_move(&ui, 2);
                break;
        }
    }
}

void test_task(void *pvParameters)
{
    // 初始化UI
    init_menu_ui(&ui);
    // 初始化控制器
    esp_err_t err_code = init_key8knob(&Con, key_cb, knob_cb);
    ESP_LOGI(ESP_TAG, "控制器初始化: %d", err_code);

    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}