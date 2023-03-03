/*
 * @Author       : zakiuc
 * @Date         : 2023-02-23 13:55:54
 * @LastEditors  : zakiuc z2337070680@163.com
 * @LastEditTime : 2023-03-02 17:07:14
 * @FilePath     : \main\UI_menu.c
 * @Description  : menu ui
 * Copyright (c) 2023 by zakiuc z2337070680@163.com, All Rights Reserved. 
 */
#include "UI_menu.h"
#include "ESP_LOG.h"
#define ESP_TAG "UI_menu"

// 菜单图标默认样式
static lv_style_t style_menu_image_default;
// 菜单标签默认样式
static lv_style_t style_menu_label_default;
// 菜单默认样式
static lv_style_t style_menu_default;
// 导航条main默认样式
static lv_style_t style_tabbar_default;
// 导航条indicator默认样式
static lv_style_t style_tabbar_indicator_default;
// 移除菜单动画
static void menu_move_out(lv_ui_t* ui);
// 返回菜单动画
static void menu_move_back(lv_ui_t* ui);

// 菜单属性表
MenuAttr_t menu_attr_table[] = {
    {
        // 日常模式
        .menu_id = 0,
        .mode_icon.image_data = &_DailyMode_100x100,
        .mode_icon.obj_attr.x = 70,
        .mode_icon.obj_attr.y = 56,
        .mode_icon.obj_attr.w = 100,
        .mode_icon.obj_attr.h = 100,
        .mode_icon.obj_attr.rx = 50,
        .mode_icon.obj_attr.ry = 50,
        .mode_icon.obj_attr.angle = 0,
        .mode_icon.obj_attr.scrollbar = LV_SCROLLBAR_MODE_OFF,
        .mode_label.text_data = "Daily Mode",
        .mode_label.text_attr1 = "15 s",
        .mode_label.text_attr2 = "LOW",
        .mode_label.text_attr2_unit = "ppm",
        .mode_label.obj_attr.x = 20,
        .mode_label.obj_attr.y = 160,
        .mode_label.obj_attr.w = 200,
        .mode_label.obj_attr.h = 32,
        .mode_label.obj_attr.longlabel = LV_LABEL_LONG_WRAP,
    },
    {
        // 深度模式
        .menu_id = 1,
        .mode_icon.image_data = &_DeepMode_100x100,
        .mode_icon.obj_attr.x = 317,
        .mode_icon.obj_attr.y = 56,
        .mode_icon.obj_attr.w = 100,
        .mode_icon.obj_attr.h = 100,
        .mode_icon.obj_attr.rx = 50,
        .mode_icon.obj_attr.ry = 50,
        .mode_icon.obj_attr.angle = 0,
        .mode_icon.obj_attr.scrollbar = LV_SCROLLBAR_MODE_OFF,
        .mode_label.text_data = "Deep Mode",
        .mode_label.text_attr1 = "30 s",
        .mode_label.text_attr2 = "HIGH",
        .mode_label.text_attr2_unit = "ppm",
        .mode_label.obj_attr.x = 267,
        .mode_label.obj_attr.y = 160,
        .mode_label.obj_attr.w = 200,
        .mode_label.obj_attr.h = 32,
        .mode_label.obj_attr.longlabel = LV_LABEL_LONG_WRAP,
    },
    {
        // 更多模式
        .menu_id = 2,
        .mode_icon.image_data = &_MoreMode_100x100,
        .mode_icon.obj_attr.x = 564,
        .mode_icon.obj_attr.y = 56,
        .mode_icon.obj_attr.w = 100,
        .mode_icon.obj_attr.h = 100,
        .mode_icon.obj_attr.rx = 50,
        .mode_icon.obj_attr.ry = 50,
        .mode_icon.obj_attr.angle = 0,
        .mode_icon.obj_attr.scrollbar = LV_SCROLLBAR_MODE_OFF,
        .mode_label.text_data = "More Mode",
        .mode_label.text_attr1 = "35 s",
        .mode_label.text_attr2 = "MID",
        .mode_label.text_attr2_unit = "ppm",
        .mode_label.obj_attr.x = 514,
        .mode_label.obj_attr.y = 160,
        .mode_label.obj_attr.w = 200,
        .mode_label.obj_attr.h = 32,
        .mode_label.obj_attr.longlabel = LV_LABEL_LONG_WRAP,
    },
    {
        .menu_id = 3,
        .mode_icon.image_data = &_DeepMode_100x100,
        .mode_icon.obj_attr.x = 811,
        .mode_icon.obj_attr.y = 56,
        .mode_icon.obj_attr.w = 100,
        .mode_icon.obj_attr.h = 100,
        .mode_icon.obj_attr.rx = 50,
        .mode_icon.obj_attr.ry = 50,
        .mode_icon.obj_attr.angle = 0,
        .mode_icon.obj_attr.scrollbar = LV_SCROLLBAR_MODE_OFF,
        .mode_label.text_data = "Test2 Mode",
        .mode_label.text_attr1 = "36 s",
        .mode_label.text_attr2 = "MID",
        .mode_label.text_attr2_unit = "ppm",
        .mode_label.obj_attr.x = 761,
        .mode_label.obj_attr.y = 160,
        .mode_label.obj_attr.w = 200,
        .mode_label.obj_attr.h = 32,
        .mode_label.obj_attr.longlabel = LV_LABEL_LONG_WRAP,
    }
};

/**
 * @brief    初始化默认样式(各个ui元素)
 * @param    (void)
 * @return   (void)
 */
void init_style(void)
{
    if (style_menu_default.prop_cnt > 1){
        lv_style_reset(&style_menu_default);
    }else{
        lv_style_init(&style_menu_default);
    }
    // 设置背景颜色为 黑色
    lv_style_set_bg_color(&style_menu_default, lv_color_make(0x00, 0x00, 0x00));
    // 设置背景透明度为 255
    lv_style_set_bg_opa(&style_menu_default, 255);

    if (style_menu_image_default.prop_cnt > 1){
        lv_style_reset(&style_menu_image_default);
    }else{
        lv_style_init(&style_menu_image_default);
    }
    lv_style_set_shadow_width(&style_menu_image_default, 0);
    lv_style_set_shadow_color(&style_menu_image_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_shadow_opa(&style_menu_image_default, 0);
    lv_style_set_shadow_spread(&style_menu_image_default, 0);
    lv_style_set_shadow_ofs_x(&style_menu_image_default, 0);
    lv_style_set_shadow_ofs_y(&style_menu_image_default, 0);
    lv_style_set_img_recolor(&style_menu_image_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_img_recolor_opa(&style_menu_image_default, 0);
    lv_style_set_img_opa(&style_menu_image_default, 255);
   

    
    if (style_menu_label_default.prop_cnt > 1){
        lv_style_reset(&style_menu_label_default);
    }else{
        lv_style_init(&style_menu_label_default);
    }
    lv_style_set_radius(&style_menu_label_default, 0);
    lv_style_set_bg_color(&style_menu_label_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_bg_grad_color(&style_menu_label_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_bg_grad_dir(&style_menu_label_default, LV_GRAD_DIR_NONE);
    lv_style_set_bg_opa(&style_menu_label_default, 0);
    lv_style_set_shadow_width(&style_menu_label_default, 0);
    lv_style_set_shadow_color(&style_menu_label_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_shadow_opa(&style_menu_label_default, 0);
    lv_style_set_shadow_spread(&style_menu_label_default, 0);
    lv_style_set_shadow_ofs_x(&style_menu_label_default, 0);
    lv_style_set_shadow_ofs_y(&style_menu_label_default, 0);
    lv_style_set_text_color(&style_menu_label_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_menu_label_default, &lv_font_montserrat_22);
    lv_style_set_text_letter_space(&style_menu_label_default, 2);
    lv_style_set_text_line_space(&style_menu_label_default, 0);
    lv_style_set_text_align(&style_menu_label_default, LV_TEXT_ALIGN_CENTER);
    lv_style_set_pad_left(&style_menu_label_default, 0);
    lv_style_set_pad_right(&style_menu_label_default, 0);
    lv_style_set_pad_top(&style_menu_label_default, 4);
    lv_style_set_pad_bottom(&style_menu_label_default, 0);


    if (style_tabbar_default.prop_cnt > 1){
        lv_style_reset(&style_tabbar_default);
    }else{
        lv_style_init(&style_tabbar_default);
    }
    lv_style_set_radius(&style_tabbar_default, 10);
	lv_style_set_bg_color(&style_tabbar_default, lv_color_make(0x77, 0x78, 0x79));
	lv_style_set_bg_grad_color(&style_tabbar_default, lv_color_make(0x39, 0x3c, 0x41));
	lv_style_set_bg_grad_dir(&style_tabbar_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_tabbar_default, 235);
	lv_style_set_shadow_width(&style_tabbar_default, 1);
	lv_style_set_shadow_color(&style_tabbar_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_shadow_opa(&style_tabbar_default, 255);
	lv_style_set_shadow_spread(&style_tabbar_default, 0);
	lv_style_set_shadow_ofs_x(&style_tabbar_default, 0);
	lv_style_set_shadow_ofs_y(&style_tabbar_default, 0);


    if (style_tabbar_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_tabbar_indicator_default);
	else
		lv_style_init(&style_tabbar_indicator_default);
	lv_style_set_radius(&style_tabbar_indicator_default, 10);
	lv_style_set_bg_color(&style_tabbar_indicator_default, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_color(&style_tabbar_indicator_default, lv_color_make(0x00, 0xa1, 0xb5));
	lv_style_set_bg_grad_dir(&style_tabbar_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_tabbar_indicator_default, 255);
}


/**
 * @brief    初始化菜单节点
 * @param    (lv_ui_t) *ui : ui结构体(包含菜单页所有项目)
 * @param    (Menu_t) mode : 显示模式
 * @return   (void)
 */
void init_menu_obj(lv_ui_t *ui, Menu_t mode)
{
    // 图片
    mode->menu_obj.mode_icon.image = lv_img_create(ui->Screen);
    lv_obj_set_pos(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.obj_attr.x, mode->menu_obj.mode_icon.obj_attr.y);
    lv_obj_set_size(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.obj_attr.w, mode->menu_obj.mode_icon.obj_attr.h);
    lv_obj_set_scrollbar_mode(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.obj_attr.scrollbar);
    lv_obj_add_style(mode->menu_obj.mode_icon.image, &style_menu_image_default, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_img_set_src(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.image_data);
    lv_img_set_pivot(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.obj_attr.rx, mode->menu_obj.mode_icon.obj_attr.ry);
    lv_img_set_angle(mode->menu_obj.mode_icon.image, mode->menu_obj.mode_icon.obj_attr.angle);

    // 标签
    mode->menu_obj.mode_label.label = lv_label_create(ui->Screen);
    lv_obj_set_pos(mode->menu_obj.mode_label.label, mode->menu_obj.mode_label.obj_attr.x, mode->menu_obj.mode_label.obj_attr.y);
    lv_obj_set_size(mode->menu_obj.mode_label.label, mode->menu_obj.mode_label.obj_attr.w, mode->menu_obj.mode_label.obj_attr.h);
    lv_obj_set_scrollbar_mode(mode->menu_obj.mode_label.label, mode->menu_obj.mode_label.obj_attr.scrollbar);
    lv_label_set_text(mode->menu_obj.mode_label.label, mode->menu_obj.mode_label.text_data);
    // 设置标签长文本模式
    lv_label_set_long_mode(mode->menu_obj.mode_label.label, mode->menu_obj.mode_label.obj_attr.longlabel);
    lv_obj_add_style(mode->menu_obj.mode_label.label, &style_menu_label_default, LV_PART_MAIN | LV_STATE_DEFAULT);
}


void OpenMenu(lv_ui_t* ui)
{
    if(ui->level == 0)
    {
        // 移除的动画
        ui->level++;
        menu_move_out(ui);
        // 菜单内容
    }
}


void BackMenu(lv_ui_t* ui)
{
    if(ui->level == 1)
    {
        // 移除的动画
        ui->level--;
        menu_move_back(ui);
        // 菜单内容
    }
}


/**
 * @brief    添加动画并立刻执行
 * @param    (lv_obj_t*) obj : 目标元素
 * @param    (lv_anim_exec_xcb_t) exec_cb : 回调函数
 * @param    (u16) time : 动画长度(ms)
 * @param    (lv_coord_t) start : 起始值(x轴坐标)
 * @param    (lv_coord_t) end : 结束值(x轴坐标)
 * @param    (lv_anim_path_cb_t) path_cb : 动画计算方法
 * @return   (void)
 */
void obj_add_anim(lv_obj_t* obj, lv_anim_exec_xcb_t exec_cb, u16 time, lv_coord_t start, lv_coord_t end, lv_anim_path_cb_t path_cb)
{
    lv_anim_t anim;

    lv_anim_init(&anim);
        
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)exec_cb);
    lv_anim_set_var(&anim, obj);

    // 设置动画长度(ms)
    lv_anim_set_time(&anim, time);

    anim.start_value = start; //起始值
    anim.end_value = end;     //结束值
    anim.path_cb = path_cb;   //动画计算方法
    anim.playback_time = 0;   //回放时间设为0不执行动画回放

    lv_anim_start(&anim); //开启动画
}


/**
 * @brief    设置obj x轴位置
 * @param    (lv_obj_t*) obj : 元素
 * @param    (lv_coord_t) x : x轴位置
 * @return   (void)
 */
void lv_obj_set_x_anim(lv_obj_t* obj, lv_coord_t x)
{
    lv_obj_set_pos(obj, x, lv_obj_get_y(obj));
}


/**
 * @brief    设置obj y轴位置
 * @param    (lv_obj_t*) obj : 元素
 * @param    (lv_coord_t) y : y轴位置
 * @return   (void)
 */
void lv_obj_set_y_anim(lv_obj_t* obj, lv_coord_t y)
{
    lv_obj_set_pos(obj, lv_obj_get_x(obj), y);
}


/**
 * @brief    导航条移动动画效果结束的回调函数
 * @param    (void) *obj : 待操作元素
 * @param    (s32) v : (x轴)起始位置
 * @return   (void)
 */
void menu_bar_anim_cb(void *obj, s32 v)
{
    lv_bar_set_start_value(obj, v, LV_ANIM_ON);
    lv_bar_set_value(obj, 30 + v, LV_ANIM_ON);
}


/**
 * @brief    导航条同步
 * @param    (lv_ui_t*) ui : 目标ui
 * @param    (u8) now : 当前索引号
 * @param    (u8) target : 目标索引号
 * @return   (void)
 */
void tabbar_sync(lv_ui_t* ui, u8 now, u8 target)
{
    obj_add_anim(
        ui->tabbar,                             // 动画对象
        (lv_anim_exec_xcb_t)menu_bar_anim_cb,   // 动画函数
        lv_anim_speed_to_time(100, 0, 42),      // 动画速度
        now*30,                                 // 起始值
        target*30,                              // 结束值
        lv_anim_path_bounce                     // 动画特效:模拟弹性物体下落
    );
}


/**
 * @brief    打开菜单项的进入动画
 * @param    (lv_ui_t*) ui : ui对象
 * @return   (void)
 */
static void menu_move_out(lv_ui_t* ui)
{
    s16 target_pos_buff[] = {-101, 273}; 
    lv_obj_t* act_scr = lv_scr_act();
    lv_disp_t* d = lv_obj_get_disp(act_scr);
    if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
    {
        Menu_t node = ListFind(ui->menu_head, ui->index);
        
        lv_obj_refr_pos(node->menu_obj.mode_icon.image);
        lv_obj_refr_pos(node->menu_obj.mode_label.label);

        // 导航栏
        lv_obj_add_flag(ui->tabbar, LV_OBJ_FLAG_HIDDEN);

        // 图标移动
        obj_add_anim(
            node->menu_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_y_anim,
            MENU_OUT_TIME,
            MODE_ICON_DEFAULT_POS_Y,
            target_pos_buff[0],
            lv_anim_path_ease_out
        );

        // 标签移动
        obj_add_anim(
            node->menu_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_y_anim,
            MENU_MOVE_TIME,
            MODE_LABEL_DEFAULT_POS_Y,
            target_pos_buff[1],
            lv_anim_path_ease_out
        );
        ESP_LOGI(ESP_TAG, "Menu open.");
    }
}


static void menu_move_back(lv_ui_t* ui)
{
    s16 target_pos_buff[] = {-101, 273}; 
    lv_obj_t* act_scr = lv_scr_act();
    lv_disp_t* d = lv_obj_get_disp(act_scr);
    if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr))
    {
        Menu_t node = ListFind(ui->menu_head, ui->index);
        
        lv_obj_refr_pos(node->menu_obj.mode_icon.image);
        lv_obj_refr_pos(node->menu_obj.mode_label.label);

        // 图标移动
        obj_add_anim(
            node->menu_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_y_anim,
            MENU_OUT_TIME,
            target_pos_buff[0],
            MODE_ICON_DEFAULT_POS_Y,
            lv_anim_path_ease_out
        );

        // 标签移动
        obj_add_anim(
            node->menu_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_y_anim,
            MENU_MOVE_TIME,
            target_pos_buff[1],
            MODE_LABEL_DEFAULT_POS_Y,
            lv_anim_path_ease_out
        );

        // 导航栏
        lv_obj_clear_flag(ui->tabbar, LV_OBJ_FLAG_HIDDEN);
        ESP_LOGI(ESP_TAG, "Menu back.");
    }
}


/**
 * @brief    移动菜单(单步)
 * @param    (lv_ui_t*) ui : ui对象
 * @param    (u8) direction : 方向  MENU_MOVE_ON(右) MENU_MOVE_BACK(左)
 * @return   (u8) 目前的菜单索引
 */
u8 menu_move_step(lv_ui_t* ui, u8 direction)
{
    s16 target_pos_buff[] = {240+1, -101, 240+1, -201}; 
    lv_obj_t* act_scr = lv_scr_act();
    lv_disp_t* d = lv_obj_get_disp(act_scr);
    if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr) && (ui->level == 0))
    {
        Menu_t node = ListFind(ui->menu_head, ui->index);
        
        MenuAttr_t next_obj;
        if (direction == MENU_MOVE_ON)
        {
            next_obj = node->next->menu_obj;
        }
        else if(direction == MENU_MOVE_BACK)
        {
            next_obj = node->previous->menu_obj;
        }
        else
        {
            return 25;
        }
        lv_obj_refr_pos(node->menu_obj.mode_icon.image);
        lv_obj_refr_pos(node->menu_obj.mode_label.label);
        lv_obj_refr_pos(next_obj.mode_icon.image);
        lv_obj_refr_pos(next_obj.mode_label.label);

        // 图标移动
        obj_add_anim(
            node->menu_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            MODE_ICON_DEFAULT_POS_X,
            0 + target_pos_buff[direction],
            lv_anim_path_ease_out
        );
        obj_add_anim(
            next_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            0 + target_pos_buff[direction == 1 ? 0 : 1],
            MODE_ICON_DEFAULT_POS_X,
            lv_anim_path_bounce
        );

        // 标签移动
        obj_add_anim(
            node->menu_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            MODE_LABEL_DEFAULT_POS_X,
            0 + target_pos_buff[direction+2],
            lv_anim_path_ease_out
        );
        obj_add_anim(
            next_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            0 + target_pos_buff[(direction == 1 ? 0 : 1) + 2],
            MODE_LABEL_DEFAULT_POS_X,
            lv_anim_path_bounce
        );

        // 导航栏
        tabbar_sync(ui, node->menu_obj.menu_id, next_obj.menu_id);
        ESP_LOGI(ESP_TAG, "Menu moved.");
        ui->index = next_obj.menu_id;
        ui->menu_node = ListFind(ui->menu_head, ui->index);
        return next_obj.menu_id;
    }
    return 25;
}


/**
 * @brief    移动菜单
 * @param    (lv_ui_t*) ui : ui 对象
 * @param    (u8) target : 目标菜单索引
 * @return   (u8) 当前菜单索引
 */
u8 menu_move(lv_ui_t* ui, u8 target)
{
    if(ui->index == target){
        return 25;
    }
    s16 target_pos_buff[] = {240+1, -101, 240+1, -201}; 
    lv_obj_t* act_scr = lv_scr_act();
    lv_disp_t* d = lv_obj_get_disp(act_scr);
    if (d->prev_scr == NULL && (d->scr_to_load == NULL || d->scr_to_load == act_scr) && (ui->level == 0))
    {
        Menu_t node = ListFind(ui->menu_head, ui->index);
        Menu_t target_node = ListFind(ui->menu_head, target);
        MenuAttr_t next_obj;
        u8 direction;
        if(FindShortcut(node, target_node)){
            direction = MENU_MOVE_ON;
        }else{
            direction = MENU_MOVE_BACK;
        }
        next_obj = target_node->menu_obj;
        lv_obj_refr_pos(node->menu_obj.mode_icon.image);
        lv_obj_refr_pos(node->menu_obj.mode_label.label);
        lv_obj_refr_pos(next_obj.mode_icon.image);
        lv_obj_refr_pos(next_obj.mode_label.label);

        // 图标移动
        obj_add_anim(
            node->menu_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            MODE_ICON_DEFAULT_POS_X,
            0 + target_pos_buff[direction],
            lv_anim_path_ease_out
        );
        obj_add_anim(
            next_obj.mode_icon.image,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            0 + target_pos_buff[direction == 1 ? 0 : 1],
            MODE_ICON_DEFAULT_POS_X,
            lv_anim_path_bounce
        );

        // 标签移动
        obj_add_anim(
            node->menu_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            MODE_LABEL_DEFAULT_POS_X,
            0 + target_pos_buff[direction+2],
            lv_anim_path_ease_out
        );
        obj_add_anim(
            next_obj.mode_label.label,
            (lv_anim_exec_xcb_t)lv_obj_set_x_anim,
            MENU_MOVE_TIME,
            0 + target_pos_buff[(direction == 1 ? 0 : 1) + 2],
            MODE_LABEL_DEFAULT_POS_X,
            lv_anim_path_bounce
        );

        // 导航栏
        tabbar_sync(ui, node->menu_obj.menu_id, next_obj.menu_id);
        ESP_LOGI(ESP_TAG, "Menu moved.");
        ui->index = next_obj.menu_id;
        ui->menu_node = ListFind(ui->menu_head, ui->index);
        return next_obj.menu_id;
    }
    return 25;
}


/**
 * @brief    初始化菜单ui
 * @param    (lv_ui_t*) ui : ui对象
 * @return   (void)
 */
void init_menu_ui(lv_ui_t* ui)
{
    Menu_t daily_node = CreateLinkList(menu_attr_table[0]);
    Menu_t deep_node = ListPushEnd(daily_node, menu_attr_table[1]);
    Menu_t more_node = ListPushEnd(daily_node, menu_attr_table[2]);

    ui->menu_head = daily_node;
    ui->index = daily_node->menu_obj.menu_id;
    ui->menu_node = ListFind(ui->menu_head, ui->index);
    ui->level = 0;
    // 初始化样式
    init_style();
    // 新建屏幕
    ui->Screen = lv_scr_act();
    // 设置对象滚动条模式  关
    lv_obj_set_scrollbar_mode(ui->Screen, LV_SCROLLBAR_MODE_OFF);
    // 添加样式到 Screen
    lv_obj_add_style(ui->Screen, &style_menu_default, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 加载屏幕
    lv_scr_load(ui->Screen);
    
    init_menu_obj(ui, daily_node);
    init_menu_obj(ui, deep_node);
    init_menu_obj(ui, more_node);

    // 导航条 main
    ui->tabbar = lv_bar_create(ui->Screen);
    lv_obj_set_pos(ui->tabbar, 75, 200);
	lv_obj_set_size(ui->tabbar, 90, 8);
	lv_obj_set_scrollbar_mode(ui->tabbar, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(ui->tabbar, &style_tabbar_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    
    lv_bar_set_range(ui->tabbar, 0, 90);
    lv_bar_set_mode(ui->tabbar, LV_BAR_MODE_RANGE);
    lv_bar_set_start_value(ui->tabbar, 0, LV_ANIM_ON);
    lv_bar_set_value(ui->tabbar, 30, LV_ANIM_ON);


    lv_obj_add_style(ui->tabbar, &style_tabbar_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);
}
