/*
 * @Author       : error: git config user.name & please set dead value or install git
 * @Date         : 2023-02-21 17:50:41
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 10:33:28
 * @FilePath     : \main\main.c
 * @Description  : 
 * Copyright (c) 2023 by error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git, All Rights Reserved. 
 */
/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "lvgl_helpers.h"
#include "user_config.h"
#include "test.h"


/* 保存显示缓冲区信息 */
static lv_disp_draw_buf_t disp_buf;
/* 注册屏幕硬件驱动 */
static lv_disp_drv_t disp_drv;
/* 为LVGL创建的周期定时器 */
esp_timer_handle_t periodic_timer;
#define LV_TICK_PERIOD_MS 1

void lvgl_init();
 

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main()
{
    lvgl_init();/* lvgl初始化 包括屏幕硬件注册等 */

    // 测试线程
    xTaskCreate((TaskFunction_t)test_task,
                (const char *)"test_task",
                (u16)TEST_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)TEST_TASK_PRIO,
                (TaskHandle_t *)&TestTask_Handler);

    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        lv_task_handler();
    }
    
}





/**
 * @brief    LVGL tick
 * @param    (void) *arg : 
 * @return   (void)
 */
void lv_tick_task(void *arg)
{
    lv_tick_inc(LV_TICK_PERIOD_MS);
}
void lvgl_init()
{
    lv_init();
    /* 初始化驱动程序使用的SPI或I2C总线 */
    lvgl_driver_init();

    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    // memset(buf1,0x00ff,DISP_BUF_SIZE * sizeof(lv_color_t));
    assert(buf1 != NULL);

    /* 当不使用单色显示器时，使用双缓冲 */
    lv_color_t *buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    // memset(buf2,0x00ff,DISP_BUF_SIZE * sizeof(lv_color_t));
    assert(buf2 != NULL);

    /* 根据所选显示初始化工作缓冲区。
     * 注意:当使用单色显示时，buf2 == NULL。 */
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    /* 注册屏幕硬件驱动 */
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.full_refresh = 1;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    /* 创建并启动一个定时定时器中断来调用lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"}; 
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}