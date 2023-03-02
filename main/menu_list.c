/*
 * @Author       : zakiuc
 * @Date         : 2023-02-22 09:17:37
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 13:27:10
 * @FilePath     : \main\menu_list.c
 * @Description  : menu obj list(菜单链表)
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#include "menu_list.h"
#include "esp_log.h"

#define LOG_TAG "MENU_LIST_LOG"


#include "user_config.h"



/**
 * @brief    动态申请一个结点
 * @param    (MenuAttr_t) menu_obj : 
 * @return   (Menu_t) NewNode : 新节点
 */
Menu_t BuyListNode(MenuAttr_t menu_obj)
{
	Menu_t NewNode = (Menu_t)malloc(sizeof(LinkList));
	if (NewNode == NULL)
	{
        ESP_LOGW(LOG_TAG, "Memory allocation failure");
		return NULL;
	}
	NewNode->previous = NewNode->next = NULL;
	NewNode->menu_obj = menu_obj;
	return NewNode;
}

/**
 * @brief    创建链表头节点
 * @param    (MenuAttr_t) menu_obj : 第一个节点的内容
 * @return   (Menu_t) tempHeader : 头节点
 */
Menu_t CreateLinkList(MenuAttr_t menu_obj)
{
    Menu_t head = BuyListNode(menu_obj);
    head->menu_obj = menu_obj;
    head->previous = head;
    head->next = head;
    return head;
}

/**
 * @brief    打印出列表内容
 * @param    (Menu_t) plist : 起始节点
 * @return   (void)
 */
void Listprint(Menu_t plist)
{
	assert(plist);
	Menu_t cur = plist;
    ESP_LOGI("", "List:");
    ESP_LOGI("", "menu id\tlabel\t\ttext_attr1\ttext_attr2\r\n");
    ESP_LOGI("", "%d\t\t%s\t%s\t\t%s\r\n", cur->menu_obj.menu_id, cur->menu_obj.mode_label.text_data, cur->menu_obj.mode_label.text_attr1, cur->menu_obj.mode_label.text_attr2);
    cur = cur->next;
	while (cur != plist)
	{
        ESP_LOGI("", "%d\t\t%s\t%s\t\t%s\r\n", cur->menu_obj.menu_id, cur->menu_obj.mode_label.text_data, cur->menu_obj.mode_label.text_attr1, cur->menu_obj.mode_label.text_attr2);
		cur = cur->next;
	}
	ESP_LOGI("", "\n");
}

/**
 * @brief    尾插(在尾节点后面插入)
 * @param    (Menu_t) plist : 头节点
 * @param    (MenuAttr_t) menu_obj : 待插入的内容
 * @return   (Menu_t) NewNode : 新插入的节点
 */
Menu_t ListPushEnd(Menu_t plist, MenuAttr_t menu_obj)
{
	assert(plist);
	Menu_t NewNode = BuyListNode(menu_obj);
	Menu_t tail = plist->previous;
	tail->next = NewNode;
	NewNode->previous = tail;
	plist->previous = NewNode;
	NewNode->next = plist;
    return NewNode;
}

/**
 * @brief    尾删(删除尾节点)
 * @param    (Menu_t) plist : 头节点
 * @return   (void)
 */
void ListPopEnd(Menu_t plist)
{
	assert(plist);
	assert(plist->next != plist);
	Menu_t tail = plist->previous;
	Menu_t tail_prev = tail->previous;
	tail_prev->next = plist;
	plist->previous = tail_prev;
	free(tail);
	tail = NULL;
}

/**
 * @brief    头插(在头节点后面插入)
 * @param    (Menu_t) plist : 头节点
 * @param    (MenuAttr_t) menu_obj : 待插入内容
 * @return   (Menu_t) NewNode : 新插入的节点
 */
Menu_t ListPushHead(Menu_t plist, MenuAttr_t menu_obj)
{
	assert(plist);
	Menu_t NewNode = BuyListNode(menu_obj);
	Menu_t first = plist->next;
	plist->next = NewNode;
	NewNode->previous = plist;
	NewNode->next = first;
	first->previous = NewNode;
    return NewNode;
}

/**
 * @brief    头删(删除头节点后一位节点)
 * @param    (Menu_t) plist : 头节点
 * @return   (void)
 */
void ListPopHead(Menu_t plist)
{
	assert(plist);
	assert(plist->next != plist);
	Menu_t first = plist->next;
	Menu_t second = first->next;
	plist->next = second;
	second->previous = plist;
	free(first);
	first = NULL;
}

/**
 * @brief    根据id查找节点
 * @param    (Menu_t) plist : 头节点
 * @param    (u8) id : menu_id(菜单id)
 * @return   (Menu_t) cur : 找到的目标节点
 * @return   (void) NULL : 没找到目标节点
 */
Menu_t ListFind(Menu_t plist, u8 id)
{
	assert(plist);
	Menu_t cur = plist->next;
	while (cur != plist)
	{
		if (cur->menu_obj.menu_id == id)
        {
            return cur;
        }
		cur = cur->next;
	}
	if (cur->menu_obj.menu_id == id)
	{
		return cur;
	}
	return NULL;
}


/**
 * @brief    找两个节点之间的最短路径
 * @param    (Menu_t) start : 起始节点
 * @param    (Menu_t) target : 目标节点
 * @return   (bool) true : next		false : previous
 */
bool FindShortcut(Menu_t start, Menu_t target)
{
	assert(start);
	assert(target);
	Menu_t cur = start;
	u8 node_num_next = 0;
	u8 node_num_previous = 0;
	while(cur->next != target)
	{
		cur = cur->next;
        node_num_next++;
	}
	node_num_next++;
	cur = start;
	while(cur->previous != target)
	{
		cur = cur->previous;
        node_num_previous++;
	}
	node_num_previous++;
	if(node_num_next <= node_num_previous){
		return true;
	}else{
		return false;
	}
}


/**
 * @brief    获取列表节点数
 * @param    (Menu_t) plist : 任意节点 
 * @return   (u8) num : 节点数
 */
u8 ListNum(Menu_t plist)
{
	u8 num = 1;
	
	assert(plist);
	Menu_t cur = plist->next;
	while (cur != plist)
	{
		num++;
		cur = cur->next;
	}
	return num;
}

/**
 * @brief    在pos(节点)前插入
 * @param    (Menu_t) pos : 待插入位置的后一个节点
 * @param    (MenuAttr_t) menu_obj : 待插入节点内容 
 * @return   (Menu_t) NewNode : 新插入的节点
 */
Menu_t ListInsertPrev(Menu_t pos, MenuAttr_t menu_obj)
{
	assert(pos);
	Menu_t NewNode = BuyListNode(menu_obj);
	Menu_t PosPrev = pos->previous;
	PosPrev->next = NewNode;
	NewNode->previous = PosPrev;
	NewNode->next = pos;
	pos->previous = NewNode;
    return NewNode;
}

/**
 * @brief    在pos(节点)后插入
 * @param    (Menu_t) pos : 待插入位置的后一个节点
 * @param    (MenuAttr_t) menu_obj : 待插入节点内容 
 * @return   (Menu_t) NewNode : 新插入的节点
 */
Menu_t ListInsertNext(Menu_t pos, MenuAttr_t menu_obj)
{
	assert(pos);
	Menu_t NewNode = BuyListNode(menu_obj);
	Menu_t Next = pos->next;
	Next->previous = NewNode;
	NewNode->previous = pos;
	NewNode->next = Next;
	pos->next = NewNode;
    return NewNode;
}

/**
 * @brief    删除pos结点
 * @param    (Menu_t) pos : 待删除的节点
 * @return   (void)
 */
void ListErase(Menu_t pos)
{
	assert(pos);
	Menu_t PosPrev = pos->previous;
	Menu_t PosNext = pos->next;
	PosPrev->next = PosNext;
	PosNext->previous = PosPrev;
	free(pos);
	pos = NULL;
}