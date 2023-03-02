/*
 * @Author       : zakiuc
 * @Date         : 2023-02-28 14:31:53
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-02-28 20:21:22
 * @FilePath     : \main\controller.c
 * @Description  : 
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#include "controller.h"
#include "ESP_LOG.h"
#include <sys/time.h>
#define ESP_TAG "CONTROLLER_LOG"

/* 旋钮事件队列句柄 */
static xQueueHandle knob_intr_evt_queue = NULL;
/* 按键事件队列句柄 */
static xQueueHandle key_intr_evt_queue = NULL;
/* 定时器句柄 */
static key_timers_t key_timer;
/* 获取系统时间用的*/
struct timeval sysTime;

/**
 * @brief    gpio中断回调函数
 * @param    (void*) arg : 参数(引脚)
 * @return   (void)
 */
static void IRAM_ATTR gpio_intr_handler(void* arg)
{
    static u8 count = 0;           
    static u8 last_value_A = 0;         
    static u8 last_value_B = 0;       
    static u8 value_A = 0;               
    static u8 value_B = 0;             

    u32 gpio_num = (u32) arg;
    bool result;
    s8 facing = 0;
    // 处理旋钮
    if(count == 0){          
        last_value_A = gpio_get_level(GPIO_INPUT_KNOB_A); 
        last_value_B = gpio_get_level(GPIO_INPUT_KNOB_B);  
        count  = 1;              
    }else if(count == 1){         
        value_A = gpio_get_level(GPIO_INPUT_KNOB_A);      
        value_B = gpio_get_level(GPIO_INPUT_KNOB_B);       
        //状态判断处理
        if(((last_value_A == 0 && value_A == 1) && (last_value_B == 1 && value_B == 0)) || ((last_value_A == 1 && value_A == 0) && (last_value_B == 0 && value_B == 1)))
        {        
            facing = -1;        //左
        }else if(((last_value_A == 0 && value_A == 1) && (last_value_B == 0 && value_B == 1)) || ((last_value_A == 1 && value_A == 0) && (last_value_B == 1 && value_B == 0)))
        {  
            facing = 1;        //右
        }
        last_value_B = 2;       
        last_value_A = 2;     
        count  = 0;            
    }
    if(facing != 0)         
    {
        result = facing == 1 ? true : false;
        result = facing == -1 ? false : true;
        xQueueSendFromISR(knob_intr_evt_queue, &result, NULL);  
    }
    // 获取按键引脚电平并送入队列
    if(gpio_num == GPIO_INPUT_KEY)
    {
        result = gpio_get_level(GPIO_INPUT_KEY) == 0 ? true : false;
        xQueueSendFromISR(key_intr_evt_queue, &result, NULL);
    }
}


/**
 * @brief    按键处理函数
 * @param    (Key_t*) key : 目标按键
 * @return   (void)
 */
static void key_hand(Key_t* key)
{
    s64 current_time;
    static s64 last_time;
    gettimeofday(&sysTime, NULL);
    current_time = sysTime.tv_sec * 1000 + (sysTime.tv_usec / 1000);
    
    if(key->state == DOWN){
        // 按键按下停止单击定时器,启动长按定时器
        esp_timer_stop(key_timer.doubleclick_time_handle);
        esp_timer_start_once(key_timer.press_time_handle, KEY_PRESSED);
    }else{
        // 
        esp_timer_stop(key_timer.press_time_handle);
        if ((current_time - last_time) < KEY_CLICK){
            key->click_num++;      
            esp_timer_start_once(key_timer.doubleclick_time_handle, CLICK_DELAY_CHECK);
        }else{
            key->click_num = 0;
        }
    }
    last_time = current_time;
}


/**
 * @brief    按键长按回调函数
 * @param    (void*) arg : 参数(目标按键)
 * @return   (void)
 */
static void key_press_callback(void* arg)
{
    Key_t* args = (Key_t*)arg;
    args->cmd = PRESSED;
    args->key_callback(0, args->cmd);
    args->cmd = IDLEKEY;
}


/**
 * @brief    双击定时器回调函数
 * @param    (void*) arg : 参数(目标按键)
 * @return   (void)
 */
static void key_doubleclick_callback(void* arg)
{
    Key_t* args = (Key_t*)arg;
    if(args->click_num == 1)
    {
        args->cmd = CLICK;
    }
    if(args->click_num == 2)
    {
        args->cmd = DOUBLECLICKED;
    }
    args->key_callback(0, args->cmd);
    args->cmd = IDLEKEY;
    args->click_num = 0;
}


/**
 * @brief    消抖定时器回调函数
 * @param    (void*) arg : 参数(目标按键)
 * @return   (void)
 */
static void key_decounce_callback(void* arg)
{
    Key_t* args = (Key_t*)arg;
    if(args->value == gpio_get_level(GPIO_INPUT_KEY))
    {
        if(args->value)
        {
            // ESP_LOGI("TEST", "消抖之后确实松开了");
            args->state = UP;
        }else{
            // ESP_LOGI("TEST", "消抖之后确实按下了");
            args->state = DOWN;
        }
        key_hand(args);
    }
}


/**
 * @brief    旋钮工作线程
 * @param    (void*) arg : 参数(目标旋钮)
 * @return   (void)
 */
static void knob_work_thread(void* arg)
{
    Knob_t* args = (Knob_t*)arg;
    for(;;)
    {
        // 获取旋钮状态
        if(xQueueReceive(knob_intr_evt_queue, &args->rx_data, 10) == pdTRUE) 
        {
            if(args->rx_data){   // 左
                args->step--;
                args->toward = LEFT;
            }else{                  // 右
                args->step++;
                args->toward = RIGHT;
            }
            args->round = args->step/30;
        }
        else if(uxQueueSpacesAvailable(knob_intr_evt_queue) == 1) 
        {
            args->toward = IDLEKNOB;
        }
        args->knob_callback(args->toward, args->step);
    }
}


/**
 * @brief    按键工作线程
 * @param    (void*) arg : 参数(目标按键)
 * @return   (void)
 */
static void key_work_thread(void* arg)
{
    Key_t* args = (Key_t*)arg;
    u32 key_state;
    for(;;)
    {
        // 不断从队列中查询是否有消息
        if(xQueueReceive(key_intr_evt_queue,&key_state,portMAX_DELAY))
        {
            args->value = gpio_get_level(GPIO_INPUT_KEY);
            // 开启消抖计时定时器
            esp_timer_start_once(key_timer.decounce_time_handle, DECOUNE_TIME);        
        }
    }
}




/**
 * @brief    控制器(旋钮、按键)初始化
 * @param    (Controller_t*) Con : 控制器
 * @param    (key_callback_t) key_cb : 按键事件回调
 * @param    (knob_callback_t) knob_cb : 旋钮事件回调
 * @return   (esp_err_t) err_code : 无错误返回 ESP_OK
 */
esp_err_t init_key8knob(Controller_t* Con, key_callback_t key_cb, knob_callback_t knob_cb)
{
    // 设置gpio中断服务为边沿触发
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
    esp_err_t err_code = ESP_OK;
    // 配置旋钮A、B脚
    gpio_config_t io_conf;                             
    io_conf.intr_type = GPIO_INTR_DISABLE;               
    io_conf.mode = GPIO_MODE_INPUT;                        
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;          
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;              
    io_conf.pin_bit_mask = GPIO_INPUT_KNOB_A_MASK;       
    // 配置A脚
    err_code = gpio_config(&io_conf);
    if (err_code != ESP_OK){
        ESP_LOGE(ESP_TAG, "init_key8knob pin_A gpio_config err_code is %d\n", err_code);
        return err_code;
    } 
    io_conf.pin_bit_mask = GPIO_INPUT_KNOB_B_MASK; 
    // 配置B脚
    err_code = gpio_config(&io_conf);                     
    if (err_code != ESP_OK){
        ESP_LOGE(ESP_TAG, "init_key8knob pin_B gpio_config err_code is %d\n", err_code);
        return err_code;
    }

    // 配置按键引脚
    io_conf.intr_type = GPIO_INTR_DISABLE;                
    io_conf.mode = GPIO_MODE_INPUT;                       
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;         
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;         
    io_conf.pin_bit_mask = GPIO_INPUT_KEY_MASK;         
    // 配置引脚
    err_code = gpio_config(&io_conf);                
    if (err_code != ESP_OK){
        ESP_LOGE(ESP_TAG, "init_key8knob pin_key gpio_config err_code is %d\n", err_code);
    }

    // 注册中调回调函数,并将触发中断的GPIO口传进中断处理函数
    gpio_isr_handler_add(GPIO_INPUT_KNOB_A, gpio_intr_handler, (void *)GPIO_INPUT_KNOB_A);
    gpio_isr_handler_add(GPIO_INPUT_KEY, gpio_intr_handler, (void *)GPIO_INPUT_KEY);
    // 打开旋钮中断
    err_code = gpio_set_intr_type(GPIO_INPUT_KNOB_A, GPIO_INTR_ANYEDGE);
    if(err_code != ESP_OK)
    {
      ESP_LOGE(ESP_TAG, "init_key8knob knob gpio_set_intr_type is %d\n",err_code);
      return err_code;
    }
    // 打开按键中断
    err_code = gpio_set_intr_type(GPIO_INPUT_KEY, GPIO_INTR_ANYEDGE);
    if(err_code != ESP_OK)
    {
      ESP_LOGE(ESP_TAG, "init_key8knob key gpio_set_intr_type is %d\n",err_code);
    }

    // 创建队列
    knob_intr_evt_queue = xQueueCreate(1, sizeof(bool));
    // 创建队列
    key_intr_evt_queue = xQueueCreate(1, sizeof(bool));

    // 创建消抖定时器
    esp_timer_create_args_t esp_decounce_timer_args = {
        .callback         = key_decounce_callback,
        .arg              = &Con->key,
        .dispatch_method  = ESP_TIMER_TASK,
        .name             = "key_decounce_timer",
    };
    err_code = esp_timer_create(&esp_decounce_timer_args, &key_timer.decounce_time_handle);
    if (err_code != ESP_OK){
        ESP_LOGE(ESP_TAG, "esp_timer_create key_decounce_timer err_code is %d\n", err_code);
    }
    // 创建长按定时器
    esp_timer_create_args_t esp_press_timer_args = {
        .callback         = key_press_callback,
        .arg              = &Con->key,
        .dispatch_method  = ESP_TIMER_TASK,
        .name             = "key_press_timer",
    };
    err_code = esp_timer_create(&esp_press_timer_args, &key_timer.press_time_handle);
    if (err_code != ESP_OK){
        ESP_LOGE(ESP_TAG, "esp_timer_create key_press_timer err_code is %d\n", err_code);
    }
    // 创建双击定时器
    esp_timer_create_args_t esp_doubleclick_timer_args = {
        .callback         = key_doubleclick_callback,
        .arg              = &Con->key,
        .dispatch_method  = ESP_TIMER_TASK,
        .name             = "key_doubleclick_timer",
    };
    err_code = esp_timer_create(&esp_doubleclick_timer_args, &key_timer.doubleclick_time_handle);
    if (err_code != ESP_OK){
        ESP_LOGI(ESP_TAG, "esp_timer_create key_doubleclick_timer err_code is %d\n", err_code);
    }

    // 初始化值
    Con->knob.toward = IDLEKNOB;
    Con->knob.step = 0;
    Con->knob.round = 0;
    Con->knob.knob_callback = knob_cb;
    Con->knob.start_step = 0;
    Con->key.state = UP;
    Con->key.cmd = IDLEKEY;
    Con->key.click_num = 0;
    Con->key.key_callback = key_cb;
    gettimeofday(&sysTime, NULL);
    Con->knob.idle_time = (sysTime.tv_sec * 1000) + (sysTime.tv_usec / 1000);

    // 创建按键工作线程
    xTaskCreate(key_work_thread, "key_work_thread", 1024*4, &Con->key, 10, NULL);
    // 创建旋钮工作线程
    xTaskCreate(knob_work_thread, "knob_work_thread", 1024*4, &Con->knob, 10, NULL);
    return err_code;
}