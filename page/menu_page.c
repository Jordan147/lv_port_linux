
/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "main_page.h"
#include "menu_page.h"
#include "order_checout.h"

#define ITEM_NUM 12 // 所有菜單的數量

static lv_obj_t *list1;
static lv_obj_t *list3;
static lv_obj_t *back_btn;
static lv_obj_t *cut_btn;
static lv_obj_t *add_btn;
static lv_obj_t *ok_btn;

static lv_obj_t *D_btn;
static lv_obj_t *SD_btn;
static lv_obj_t *SF_btn;
static lv_obj_t *bell_btn;
static lv_obj_t *bill_btn;

static lv_obj_t *D_list;
static lv_obj_t *SD_list;
static lv_obj_t *SF_list;
static lv_obj_t *order_list;

static lv_obj_t *order_label;

int order_count = 0;
float order_total = 0;
int cur_item_index;

int menu_dis_num = M_SF_LIST;
int last_menu_dis_num = M_SF_LIST;
int order_check_num = O_IDLE;

LV_IMG_DECLARE(D_item0001);
LV_IMG_DECLARE(D_item0002);
LV_IMG_DECLARE(D_item0003);
LV_IMG_DECLARE(D_item0004);
LV_IMG_DECLARE(SD_item0001);
LV_IMG_DECLARE(SD_item0002);
LV_IMG_DECLARE(SD_item0003);
LV_IMG_DECLARE(SD_item0004);
LV_IMG_DECLARE(SF_item0001);
LV_IMG_DECLARE(SF_item0002);
LV_IMG_DECLARE(SF_item0003);
LV_IMG_DECLARE(SF_item0004);
LV_IMG_DECLARE(Drink);
LV_IMG_DECLARE(Side_Dishes);
LV_IMG_DECLARE(Staple_Food);
LV_IMG_DECLARE(bell);
LV_IMG_DECLARE(bill);

static void menu_list_event(lv_event_t *e);
static void menu_order_event(lv_event_t *e);

/**********************
 *  STYLE
 **********************/

static lv_style_t bg_opa_100;
static lv_style_t bg_opa_0;
static lv_style_t shop_cir_btn;
static lv_style_t text_font_opa_50;

void shop_item_style()
{
    lv_style_init(&bg_opa_100);
    lv_style_set_pad_all(&bg_opa_100, 1);

    lv_style_init(&bg_opa_0);
    lv_style_set_pad_all(&bg_opa_0, 1);
    lv_style_set_bg_opa(&bg_opa_0, 0);

    lv_style_init(&text_font_opa_50);
    lv_style_set_text_opa(&text_font_opa_50, LV_OPA_50);
    lv_style_set_text_font(&text_font_opa_50, &lv_jf_16);

    lv_style_init(&shop_cir_btn);
    lv_style_set_bg_color(&shop_cir_btn, lv_color_hex(0x343839));
    lv_style_set_radius(&shop_cir_btn, LV_RADIUS_CIRCLE);
}

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_obj_t *create_shop_item(lv_obj_t *parent, const void *img_src, const char *name, const char *category,
                                  float price, int index)
{
    char tmp[30];

    strcpy(menu_item[index].name, name);
    menu_item[index].id = index;
    menu_item[index].price = price;
    menu_item[index].img_src = img_src;

    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_PCT(100), 110);

    static int32_t grid_col_dsc[] = {30, 40, 30, 5, LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t grid_row_dsc[] = {30, 40, 30, 5, LV_GRID_TEMPLATE_LAST};

    lv_obj_t *gird_cont = lv_obj_create(obj);
    lv_obj_remove_style_all(gird_cont);
    lv_obj_set_size(gird_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(gird_cont, grid_col_dsc, grid_row_dsc);

    lv_obj_t *img = lv_image_create(gird_cont);
    lv_image_set_src(img, img_src);
    lv_obj_set_grid_cell(img, LV_GRID_ALIGN_START, 0, 3, LV_GRID_ALIGN_END, 0, 3);

    lv_obj_t *label;
    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    lv_label_set_text(label, name);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 4, 2, LV_GRID_ALIGN_CENTER, 0, 1);

    label = lv_label_create(gird_cont);
    lv_label_set_text(label, category);
    lv_obj_add_style(label, &text_font_opa_50, 0);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 4, 2, LV_GRID_ALIGN_START, 1, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp, "$%.0f", (double)price);
    lv_label_set_text(label, tmp);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 4, 1, LV_GRID_ALIGN_CENTER, 2, 1);

    lv_obj_t *add_btn = lv_button_create(gird_cont);
    lv_obj_set_size(add_btn, 20, 20);
    lv_obj_set_style_bg_image_src(add_btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_style(add_btn, &shop_cir_btn, LV_PART_MAIN);
    lv_obj_set_grid_cell(add_btn, LV_GRID_ALIGN_CENTER, 5, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_add_flag(add_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(add_btn, menu_list_event, LV_EVENT_PRESSED, index);

    lv_obj_add_style(obj, &bg_opa_100, LV_PART_MAIN);

    return gird_cont;
}

static lv_obj_t *create_order_item(lv_obj_t *parent, int index)
{
    char tmp1[30];
    char tmp2[30];
    char tmp3[30];
    char tmp4[30];

    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_PCT(100), 50);

    static int32_t grid_col_dsc[] = {20, LV_GRID_FR(1), 40, 40, LV_GRID_TEMPLATE_LAST};
    static int32_t grid_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *gird_cont = lv_obj_create(obj);
    lv_obj_remove_style_all(gird_cont);
    lv_obj_set_size(gird_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(gird_cont, grid_col_dsc, grid_row_dsc);

    lv_obj_t *label;
    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp1, "%d", order_item[index].id);
    lv_label_set_text(label, tmp1);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp2, "%s", order_item[index].name);
    lv_label_set_text(label, tmp2);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp3, "$%.0f", order_item[index].price);
    lv_label_set_text(label, tmp3);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp4, "%d", order_item[index].quantity);
    lv_label_set_text(label, tmp4);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 3, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_add_style(obj, &bg_opa_100, LV_PART_MAIN);
    order_total = order_total + order_item[index].price * order_item[index].quantity;

    return gird_cont;
}

static lv_obj_t *create_order_total(lv_obj_t *parent)
{
    char tmp[30];

    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, LV_PCT(100), 50);

    static int32_t grid_col_dsc[] = {20, LV_GRID_FR(1), 40, 40, LV_GRID_TEMPLATE_LAST};
    static int32_t grid_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_t *gird_cont = lv_obj_create(obj);
    lv_obj_remove_style_all(gird_cont);
    lv_obj_set_size(gird_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(gird_cont, grid_col_dsc, grid_row_dsc);

    lv_obj_t *label;
    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    lv_label_set_text(label, "total");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 0, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp, "%.0f", order_total);
    lv_label_set_text(label, tmp);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 2, LV_GRID_ALIGN_START, 0, 1);

    lv_obj_add_style(obj, &bg_opa_100, LV_PART_MAIN);

    return gird_cont;
}

void menu_item_num(lv_obj_t *parent)
{
    lv_obj_set_style_text_font(order_label, &lv_jf_16, 0);
    lv_label_set_text_fmt(order_label, "%" LV_PRIu32, order_count);
}

void m_D_page(lv_obj_t *parent)
{
    D_list = lv_obj_create(parent);
    lv_obj_set_size(D_list, LV_PCT(100), 350);
    lv_obj_set_flex_flow(D_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(D_list, 1);

    lv_obj_add_style(D_list, &bg_opa_0, LV_PART_MAIN); // 矩形背景部分

    create_shop_item(D_list, &D_item0001, "可樂", "描述", 25, 0);
    create_shop_item(D_list, &D_item0002, "奶茶", "描述", 25, 1);
    create_shop_item(D_list, &D_item0003, "綠茶", "描述", 25, 2);
    create_shop_item(D_list, &D_item0003, "綠茶", "描述", 25, 3);
}

void m_SD_page(lv_obj_t *parent)
{
    SD_list = lv_obj_create(parent);
    lv_obj_set_size(SD_list, LV_PCT(100), 350);
    lv_obj_set_flex_flow(SD_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(SD_list, 1);

    lv_obj_add_style(SD_list, &bg_opa_0, LV_PART_MAIN); // 矩形背景部分

    create_shop_item(SD_list, &SD_item0001, "雞腿", "描述", 60, 4);
    create_shop_item(SD_list, &SD_item0002, "滷蛋", "描述", 15, 5);
    create_shop_item(SD_list, &SD_item0003, "小菜", "描述", 40, 6);
    create_shop_item(SD_list, &SD_item0004, "薯條", "描述", 60, 7);
}

void m_SF_page(lv_obj_t *parent)
{
    SF_list = lv_obj_create(parent);
    lv_obj_set_size(SF_list, LV_PCT(100), 350);
    lv_obj_set_flex_flow(SF_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(SF_list, 1);

    lv_obj_add_style(SF_list, &bg_opa_0, LV_PART_MAIN); // 矩形背景部分

    create_shop_item(SF_list, &SF_item0001, "小滷肉飯", "描述", 35, 8);
    create_shop_item(SF_list, &SF_item0002, "滷肉麵", "描述", 60, 9);
    create_shop_item(SF_list, &SF_item0003, "滷肉麵", "描述", 60, 10);
    create_shop_item(SF_list, &SF_item0004, "滷肉麵", "描述", 60, 11);
}

void m_item_page(lv_obj_t *parent)
{
    char tmp[30];

    list3 = lv_obj_create(parent);
    lv_obj_set_size(list3, LV_PCT(100), 350);
    lv_obj_set_flex_flow(list3, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(list3, 1);

    static int32_t grid_col_dsc[] = {LV_GRID_FR(1), 40, 40, 40, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t grid_row_dsc[] = {120, 30, 30, 50, 30, 30, LV_GRID_TEMPLATE_LAST};

    lv_obj_t *gird_cont = lv_obj_create(list3);
    lv_obj_remove_style_all(gird_cont);
    lv_obj_set_size(gird_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(gird_cont, grid_col_dsc, grid_row_dsc);

    back_btn = lv_button_create(gird_cont);
    lv_obj_set_size(back_btn, 20, 20);
    lv_obj_set_style_bg_image_src(back_btn, LV_SYMBOL_LEFT, 0);
    lv_obj_add_style(back_btn, &shop_cir_btn, LV_PART_MAIN);
    lv_obj_set_grid_cell(back_btn, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 1);
    lv_obj_add_flag(back_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(back_btn, menu_list_event, LV_EVENT_PRESSED, 0);

    lv_obj_t *img = lv_image_create(gird_cont);
    lv_image_set_src(img, menu_item[cur_item_index].img_src);
    lv_obj_set_grid_cell(img, LV_GRID_ALIGN_START, 1, 3, LV_GRID_ALIGN_START, 0, 1);

    lv_obj_t *label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    lv_label_set_text(label, menu_item[cur_item_index].name);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 4, LV_GRID_ALIGN_START, 1, 1);

    label = lv_label_create(gird_cont);
    lv_obj_set_style_text_font(label, &lv_jf_16, 0);
    sprintf(tmp, "$%.0f", menu_item[cur_item_index].price);
    lv_label_set_text(label, tmp);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 4, LV_GRID_ALIGN_START, 2, 1);

    cut_btn = lv_button_create(gird_cont);
    lv_obj_set_size(cut_btn, 20, 20);
    lv_obj_set_style_bg_image_src(cut_btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_style(cut_btn, &shop_cir_btn, LV_PART_MAIN);
    lv_obj_set_grid_cell(cut_btn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 4, 1);
    lv_obj_add_flag(cut_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(cut_btn, menu_order_event, LV_EVENT_PRESSED, cur_item_index);

    // lv_obj_t *order_cont = lv_obj_create(gird_cont);
    order_label = lv_label_create(gird_cont);
    menu_item_num(order_label);
    lv_obj_set_grid_cell(order_label, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    add_btn = lv_button_create(gird_cont);
    lv_obj_set_size(add_btn, 20, 20);
    lv_obj_set_style_bg_image_src(add_btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_style(add_btn, &shop_cir_btn, LV_PART_MAIN);
    lv_obj_set_grid_cell(add_btn, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_CENTER, 4, 1);
    lv_obj_add_flag(add_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(add_btn, menu_order_event, LV_EVENT_PRESSED, cur_item_index);

    ok_btn = lv_button_create(gird_cont);
    lv_obj_set_size(ok_btn, 20, 20);
    lv_obj_set_style_bg_image_src(ok_btn, LV_SYMBOL_OK, 0);
    lv_obj_add_style(ok_btn, &shop_cir_btn, LV_PART_MAIN);
    lv_obj_set_grid_cell(ok_btn, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_add_flag(ok_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(ok_btn, menu_order_event, LV_EVENT_PRESSED, cur_item_index);
}

void m_bill_page(lv_obj_t *parent)
{
    order_total = 0;
    order_list = lv_obj_create(parent);
    lv_obj_set_size(order_list, LV_PCT(100), 350);
    lv_obj_set_flex_flow(order_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(order_list, 1);
    lv_obj_add_style(order_list, &bg_opa_100, LV_PART_MAIN); // 矩形背景部分

    if (orderCount > 0)
    {
        for (int i = 0; i < ITEM_NUM; i++)
        {
            LV_LOG_USER("i : %d\n", i);
            for (int j = 0; j < orderCount; j++)
            {
                LV_LOG_USER("j : %d\n", j);
                if (menu_item[i].id == order_item[j].menu_id)
                {
                    create_order_item(order_list, j);
                }
            }
        }

        create_order_total(order_list);
    }
}

void m_dis_last_sel()
{
    if (last_menu_dis_num == M_SF_LIST)
    {
        lv_obj_delete(SF_list);
    }
    else if (last_menu_dis_num == M_SD_LIST)
    {
        lv_obj_delete(SD_list);
    }
    else if (last_menu_dis_num == M_D_LIST)
    {
        lv_obj_delete(D_list);
    }
    else if (last_menu_dis_num == M_BILL)
    {
        lv_obj_delete(order_list);
    }
}

static void menu_list_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn_tar = lv_event_get_target(e);
    int32_t index = lv_event_get_user_data(e);

    if (code == LV_EVENT_PRESSED)
    {
        play_click_tone();

        if (btn_tar == back_btn)
        {
            menu_dis_num = last_menu_dis_num;
            lv_obj_delete(list3);
            menu_display(list1);
        }
        else
        {
            m_dis_last_sel();

            cur_item_index = index;

            if (orderCount > 0)
            {
                for (int i = 0; i < orderCount; i++)
                {
                    LV_LOG_USER("i : %d\n", i);
                    LV_LOG_USER("orderCount : %d\n", orderCount);
                    if (menu_item[cur_item_index].id == order_item[i].menu_id)
                    {
                        order_count = order_item[i].quantity;
                        break;
                    }
                    else
                    {
                        order_count = 0;
                    }
                }
            }

            menu_dis_num = M_ITEM;
            menu_display(list1);
        }
    }
}

void order_item_check(int32_t index)
{
    int count = 0;
    bool new_tiem = false;
    while (order_check_num < O_WORKING)
    {
        switch (order_check_num)
        {
        case O_IDLE:
        {
            if (count < cur_item_index)
            {
                if (menu_item[cur_item_index].id == order_item[count].menu_id)
                {
                    LV_LOG_USER("old_item\n");
                    order_check_num = O_OLD_ORDER;
                }
                else
                {
                    LV_LOG_USER("old_item2\n");
                    count++;
                    order_check_num = O_IDLE2;
                }
            }
        }
        break;

        case O_IDLE2:
        {
            if (count < cur_item_index)
            {
                LV_LOG_USER("O_IDLE2\n");
                order_check_num = O_IDLE;
            }
            else
            {
                LV_LOG_USER("new_item\n");
                order_check_num = O_NEW_ORDER;
            }
        }
        break;

        case O_NEW_ORDER:
        {
            order_item[orderCount].menu_id = menu_item[cur_item_index].id;
            order_item[orderCount].id = orderCount + 1;
            strcpy(order_item[orderCount].name, menu_item[index].name);
            order_item[orderCount].price = menu_item[index].price;

            order_item[orderCount].quantity = order_count;
            LV_LOG_USER("orderCount:%d\n", orderCount);
            LV_LOG_USER("id:%d name:%s pice:%.0f order:%d\n", order_item[orderCount].id, order_item[orderCount].name, order_item[orderCount].price, order_item[orderCount].quantity);
            orderCount++;
            order_check_num = O_DONE;
        }
        break;

        case O_OLD_ORDER:
        {
            order_item[count].quantity = order_count;
            LV_LOG_USER("count:%d\n", orderCount);
            LV_LOG_USER("id2:%d name:%s pice:%.0f order:%d\n", order_item[count].id, order_item[count].name, order_item[count].price, order_item[count].quantity);
            order_check_num = O_DONE;
        }
        break;
        }
    }
    lv_obj_delete(list3);
    menu_dis_num = last_menu_dis_num;
    menu_display(list1);
}

static void menu_order_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn_tar = lv_event_get_target(e);
    int32_t index = lv_event_get_user_data(e);

    if (code == LV_EVENT_PRESSED)
    {
        play_click_tone();

        if (btn_tar == cut_btn)
        {
            order_count--;
            if (order_count < 0)
                order_count = 0;

            lv_obj_clean(order_label);
            menu_item_num(order_label);
        }

        else if (btn_tar == add_btn)
        {
            order_count++;
            lv_obj_clean(order_label);
            menu_item_num(order_label);
        }

        else if (btn_tar == ok_btn)
        {
            order_check_num = O_IDLE;
            order_item_check(index);
        }
    }
}

void top_menu_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn_tar = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        play_click_tone();

        if (btn_tar == D_btn)
        {
            LV_LOG_USER("D_btn");
            m_dis_last_sel();
            menu_dis_num = M_D_LIST;
            last_menu_dis_num = M_D_LIST;
            menu_display(list1);
        }
        
        else if (btn_tar == SD_btn)
        {
            LV_LOG_USER("SD_btn");
            m_dis_last_sel();
            menu_dis_num = M_SD_LIST;
            last_menu_dis_num = M_SD_LIST;
            menu_display(list1);
        }

        else if (btn_tar == SF_btn)
        {
            LV_LOG_USER("SF_btn");
            m_dis_last_sel();
            menu_dis_num = M_SF_LIST;
            last_menu_dis_num = M_SF_LIST;
            menu_display(list1);
        }

        else if (btn_tar == bell_btn)
        {
            menu_dis_num = M_D_LIST;
        }

        else if (btn_tar == bill_btn)
        {
            LV_LOG_USER("M_BILL");
            m_dis_last_sel();
            menu_dis_num = M_BILL;
            last_menu_dis_num = M_BILL;
            menu_display(list1);
        }
    }
}

void top_menu(lv_obj_t *parent)
{
     D_btn = lv_button_create(parent);
    btn_pic(D_btn, &Drink, top_menu_event);

    SD_btn = lv_button_create(parent);
    btn_pic(SD_btn, &Side_Dishes, top_menu_event);

    SF_btn = lv_button_create(parent);
    btn_pic(SF_btn, &Staple_Food, top_menu_event);

    bell_btn = lv_button_create(parent);
    btn_pic(bell_btn, &bell, top_menu_event);

    bill_btn = lv_button_create(parent);
    btn_pic(bill_btn, &bill, top_menu_event);
}

void menu_display(lv_obj_t *parent)
{
    switch (menu_dis_num)
    {
    case M_D_LIST:
    {
        m_D_page(parent);
    }
    break;

    case M_SD_LIST:
    {
        m_SD_page(parent);
    }
    break;

    case M_SF_LIST:
    {
        m_SF_page(parent);
    }
    break;

    case M_ITEM:
    {
        m_item_page(parent);
    }
    break;

    case M_BILL:
    {
        m_bill_page(parent);
    }
    break;
    }
}

void menu_list(lv_obj_t *parent)
{
    list1 = lv_obj_create(parent);
    lv_obj_set_height(list1, 350);
    lv_obj_set_flex_flow(list1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(list1, 1);

    shop_item_style();
    lv_obj_add_style(list1, &bg_opa_0, LV_PART_MAIN);

    lv_obj_set_scroll_dir(list1, LV_DIR_VER);                // 垂直滾動
    lv_obj_set_scrollbar_mode(list1, LV_SCROLLBAR_MODE_OFF); // 不顯示滾動條

    menu_display(list1);
}
