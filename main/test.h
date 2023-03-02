/*
 * @Author       : zakiuc
 * @Date         : 2023-02-21 19:27:44
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-02-28 15:41:09
 * @FilePath     : \main\test.h
 * @Description  : 
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#ifndef _TEST_H_
#define _TEST_H_
#include "lvgl.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"




#define TEST_STK_SIZE 4*1024
#define TEST_TASK_PRIO 3
TaskHandle_t TestTask_Handler;
void test_task(void *pvParameters);
#endif   //_TEST_H_