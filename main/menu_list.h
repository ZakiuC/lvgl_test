/*
 * @Author       : zakiuc
 * @Date         : 2023-02-22 09:18:21
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 16:48:45
 * @FilePath     : \main\menu_list.h
 * @Description  : 
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */

#ifndef _MENU_LIST_H_
#define _MENU_LIST_H_
#include "lvgl.h"
#include "user_config.h"





/* 坐标数据 */
typedef struct
{
    // x轴
    lv_coord_t x;
    // y轴
    lv_coord_t y;

    // 宽
    lv_coord_t w;
    // 高
    lv_coord_t h;

    // 旋转中心x轴坐标
    lv_coord_t rx;
    // 旋转中心y轴坐标
    lv_coord_t ry;

    // 旋转角度
    s16 angle;

    // 滚动条模式
    lv_scrollbar_mode_t scrollbar;

    // 长文本模式
    lv_label_long_mode_t longlabel;
}ObjAttr_t;

/* 图标数据 */
typedef struct
{
    // 图标图像数据
    const void* image_data;

    // 位置 大小 滚动条模式
    ObjAttr_t obj_attr;

    // 图标对象
    lv_obj_t* image;
}Icon_t;

/* 标签数据 */
typedef struct
{
    const char* text_data;
    const char* text_attr1;
    const char* text_attr2;
    const char* text_attr2_unit;

    // 位置 大小 滚动条模式
    ObjAttr_t obj_attr;

    // 标签对象
    lv_obj_t* label;
}Label_t;

/* 菜单元素 */
typedef struct
{
    // 模式图标
    Icon_t mode_icon;
    // 模式标签
    Label_t mode_label;
    // 模式 id
    u8 menu_id;
}MenuAttr_t;

/* 链表结构体 */
typedef struct menu {
    MenuAttr_t menu_obj;
    struct menu* previous;
    struct menu* next;
}LinkList, *Menu_t;



// 动态申请一个结点
Menu_t BuyListNode(MenuAttr_t menu_obj);
// 创建链表头节点
Menu_t CreateLinkList(MenuAttr_t menu_obj);
// 打印出列表内容
void Listprint(Menu_t plist);
// 尾插(在尾节点后面插入)
Menu_t ListPushEnd(Menu_t plist, MenuAttr_t menu_obj);
// 尾删(删除尾节点)
void ListPopEnd(Menu_t plist);
// 头插(在头节点后面插入)
Menu_t ListPushHead(Menu_t plist, MenuAttr_t menu_obj);
// 头删(删除头节点后一位节点)
void ListPopHead(Menu_t plist);
// 根据id查找节点
Menu_t ListFind(Menu_t plist, u8 id);
// 找两节点最短路径
bool FindShortcut(Menu_t start, Menu_t target);
// 获取链表节点数
u8 ListNum(Menu_t plist);
// 在pos(节点)前插入
Menu_t ListInsertPrev(Menu_t pos, MenuAttr_t menu_obj);
// 在pos(节点)后插入
Menu_t ListInsertNext(Menu_t pos, MenuAttr_t menu_obj);
// 删除pos结点
void ListErase(Menu_t pos);
#endif  // _MENU_LIST_H_