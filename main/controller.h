/*
 * @Author       : zakiuc
 * @Date         : 2023-02-28 14:32:03
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-02-28 14:39:10
 * @FilePath     : \main\controller.h
 * @Description  : 
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_timer.h" 

#include "user_config.h"

#define GPIO_INPUT_KNOB_A     19
#define GPIO_INPUT_KNOB_B     20
#define GPIO_INPUT_KEY        21

#define GPIO_INPUT_KNOB_A_MASK (1ULL<<GPIO_INPUT_KNOB_A)
#define GPIO_INPUT_KNOB_B_MASK (1ULL<<GPIO_INPUT_KNOB_B)
#define GPIO_INPUT_KEY_MASK (1ULL<<GPIO_INPUT_KEY)


#define KEY_CLICK             300  // 单击等待时间(ms)
#define DECOUNE_TIME          10*1000 // 消抖时间(us)    
#define CLICK_DELAY_CHECK     150*1000 // 单击等待间隔150ms
#define KEY_PRESSED           1000*1000  // 长按等待时间(us)  


/* 定义函数指针 */
typedef void (*key_callback_t)(u8 index, u8 cmd);
typedef void (*knob_callback_t)(u8 cmd, s32 cnt);


// 旋钮事件
typedef enum
{
    IDLEKNOB = 0,       // 空闲
    RIGHT,              // 向右
    LEFT,               // 向左
}Toward_e;

// 按键状态
typedef enum
{
    UP = 0,             // 弹起(未按下)
    DOWN,               // 按下
}Key_e;

// 按键事件
typedef enum
{
    IDLEKEY = 0,         // 空闲(无命令)
    CLICK,               // 单机
    PRESSED,             // 长按
    DOUBLECLICKED,       // 双击
}KeyCmd_e;

// 旋钮数据
typedef struct
{
    Toward_e    toward;        // 当前朝向

    bool        rx_data;       // 接收中断数据

    s32     step;          // 已走步长(左负 右正)
    s16     round;         // 已转圈数(左负 右正)

    s32     start_step;    // 起始位置

    u32    idle_time;     // 开始空闲的时间
    knob_callback_t knob_callback;
}Knob_t;

// 按键数据
typedef struct
{
    Key_e       state;         // 当前按钮状态
    
    KeyCmd_e    cmd;           // 当前按键命令

    int         value;         // 按键电平数据
    u8     click_num;     // 单击次数

    key_callback_t key_callback; // 按键执行回调
}Key_t;


// 控制器
typedef struct
{
    Knob_t  knob;
    Key_t   key;
}Controller_t;


// 按键计时相关参数的结构体
typedef struct key_timers
{
  esp_timer_handle_t doubleclick_time_handle;                     // 用于计时按键按下并释放之后多少ms内,没有再次按下按键则说明整个按键动作完成
  esp_timer_handle_t decounce_time_handle;                        // 用于按键消抖的计时
  esp_timer_handle_t press_time_handle;                           // 用于按键长时的计时
} key_timers_t;


// 初始化控制器
esp_err_t init_key8knob(Controller_t* Con, key_callback_t key_cb, knob_callback_t knob_cb);
#endif // _CONTROLLER_H_