/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lvgl/lvgl.h"
#include "main_page.h"
#include "menu_page.h"
#include "order_checout.h"
#include "src/middle_ware/player_int.h"

/*********************
 *      DEFINES
 *********************/
int volume;
bool video_active;
lv_obj_t *video_cont;
lv_obj_t *video_btn;

static lv_obj_t *parent0;
static lv_obj_t *bg_img;

static lv_obj_t *main_page_bg;
static lv_obj_t *Activity_page_bg;
static lv_obj_t *lottery_page_bg;
static lv_obj_t *menu_page_bg;
static lv_obj_t *set_page_bg;

static lv_obj_t *gird_cont;

static lv_obj_t *origin_btn;
static lv_obj_t *Activity_btn;
static lv_obj_t *menu_btn;
static lv_obj_t *lottery_btn;
static lv_obj_t *home_btn;
static lv_obj_t *set_btn;

static lv_style_t btn_style;
static lv_style_t layer2_s;

LV_IMG_DECLARE(bakp);
LV_IMG_DECLARE(home);
LV_IMG_DECLARE(Activity);
LV_IMG_DECLARE(menu);
LV_IMG_DECLARE(origin);
LV_IMG_DECLARE(lottery);
LV_IMG_DECLARE(act1);
LV_IMG_DECLARE(act2);

static void mian_page(void);
static void Activity_page(void);
static void menu_page(void);
static void lottery_page(void);
static void setting_page(void);
static void video_event_cb(lv_event_t *e);
void media_player(const char * filename);
void btn_sty(int32_t w, int32_t h, lv_obj_t * btn);
void layer2_style(lv_obj_t * obj);
void test4(lv_obj_t * parent);
void set_list(lv_obj_t * parent);
void video_layer_switch_opa(bool opa);

void btn_sty(int32_t w, int32_t h, lv_obj_t *btn)
{
    lv_obj_set_size(btn, w, h);
    lv_style_init(&btn_style);
    lv_style_set_bg_opa(&btn_style, 0);
    lv_style_set_shadow_width(&btn_style, 0);
    lv_obj_add_style(btn, &btn_style, 0);
}

void layer2_style(lv_obj_t *obj)
{
    lv_style_init(&layer2_s);
    lv_style_set_bg_opa(&layer2_s, 0);
    lv_style_set_pad_all(&layer2_s, 1);
    lv_obj_add_style(obj, &layer2_s, 0);
}

void media_player(const char *filename)
{
    char * ptr = strchr(filename, '.');
    if(ptr == NULL) {
        printf("illegal media filename: %s\n", filename);
        return;
    }
    if (strcmp(ptr, ".wav") == 0 || \
        strcmp(ptr, ".mp3") == 0) {
        if (tplayer_play_url(filename) < 0)
        {
            printf("Failed to play audio: %s\n", filename);
        }
        else
        {
            printf("Playing audio: %s\n", filename);
            tplayer_play();
        }
    }
    else if (strcmp(ptr, ".mp4") == 0) {
        if (tplayer_play_url(filename) < 0)
        {
            printf("Failed to play video: %s\n", filename);
        }
        else
        {
            video_active = true;
            video_layer_switch_opa(true);
            printf("Playing video: %s\n", filename);
            tplayer_play();
        }
    }
    else
        printf("Not supported media file: %s\n", filename);
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn_tar = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        media_player(MEDIA_CLICK);

        if (btn_tar == origin_btn)
        {
            media_player(MEDIA_ADVERTISEMENT);
        }

        else if (btn_tar == Activity_btn)
        {
            Activity_page();
            lv_scr_load_anim(Activity_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
        }
        else if (btn_tar == menu_btn)
        {
            menu_page();
            lv_scr_load_anim(menu_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
        }
        else if (btn_tar == lottery_btn)
        {
            lottery_page();
            lv_scr_load_anim(lottery_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
        }

        else if (btn_tar == home_btn)
        {
            mian_page();
            lv_scr_load_anim(main_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
        }

        else if (btn_tar == set_btn)
        {
            setting_page();
            lv_scr_load_anim(set_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
        }
    }
}

void btn_pic(lv_obj_t *btn, const void *img_src, lv_event_cb_t event_cb)
{
    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_PRESSED, 0);
    lv_obj_set_style_bg_img_src(btn, img_src, 0);
    btn_sty(60, 60, btn);
}

static void bg_pic(lv_obj_t *bg_obj)
{
    bg_img = lv_img_create(bg_obj);
    lv_img_set_src(bg_img, &bakp);
    lv_obj_center(bg_img);
    lv_obj_set_size(bg_img, 272, 480);
}

static void mian_page()
{
    main_page_bg = lv_obj_create(NULL);
    bg_pic(main_page_bg);

    /*******網格分割******/
    static lv_coord_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {100, 100, 100, 100, LV_GRID_TEMPLATE_LAST};

    gird_cont = lv_obj_create(main_page_bg);
    lv_obj_set_grid_align(gird_cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_dsc_array(gird_cont, col_dsc, row_dsc);
    lv_obj_set_size(gird_cont, 272, 480);
    lv_obj_set_style_bg_opa(gird_cont, 0, 0);
    lv_obj_center(gird_cont);
    lv_obj_set_layout(gird_cont, LV_LAYOUT_GRID);

    origin_btn = lv_btn_create(gird_cont);
    lv_obj_set_grid_cell(origin_btn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    btn_pic(origin_btn, &origin, event_handler);

    Activity_btn = lv_btn_create(gird_cont);
    lv_obj_set_grid_cell(Activity_btn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    btn_pic(Activity_btn, &Activity, event_handler);

    menu_btn = lv_btn_create(gird_cont);
    lv_obj_set_grid_cell(menu_btn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1);
    btn_pic(menu_btn, &menu, event_handler);

    lottery_btn = lv_btn_create(gird_cont);
    lv_obj_set_grid_cell(lottery_btn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    btn_pic(lottery_btn, &lottery, event_handler);

    set_btn = lv_btn_create(gird_cont);
    lv_obj_set_grid_cell(set_btn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 3, 1);
    btn_pic(set_btn, LV_SYMBOL_SETTINGS, event_handler);
}

static void menu_page()
{
    /*****第一層 背景**** */
    menu_page_bg = lv_obj_create(NULL);
    bg_pic(menu_page_bg);

    /*****第二層 **** */

    parent0 = lv_obj_create(menu_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    layer2_style(cont_row);

    home_btn = lv_btn_create(cont_row);
    btn_pic(home_btn, &home, event_handler);

    Activity_btn = lv_btn_create(cont_row);
    btn_pic(Activity_btn, &Activity, event_handler);

    top_menu(cont_row);

    layer2_style(parent0);

    /*****第二層 menu**** */

    lv_obj_t *parent = lv_obj_create(parent0);
    lv_obj_set_size(parent, LV_PCT(100), LV_SIZE_CONTENT);
    layer2_style(parent);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);

    menu_list(parent);

    /*****第二層 選單 對其 menu**** */

    lv_obj_align_to(parent, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

static const lv_img_dsc_t *anim_imgs[2] = {
    &act2,
    &act1,
};

static void Activity_page()
{
    /*****第一層 背景**** */
    Activity_page_bg = lv_obj_create(NULL);
    bg_pic(Activity_page_bg);

    /*****第二層 **** */

    parent0 = lv_obj_create(Activity_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    home_btn = lv_btn_create(cont_row);
    btn_pic(home_btn, &home, event_handler);

    menu_btn = lv_btn_create(cont_row);
    btn_pic(menu_btn, &menu, event_handler);

    /*****第二層 menu**** */

    layer2_style(parent0);

    lv_obj_t *parent = lv_obj_create(parent0);
    lv_obj_set_size(parent, LV_PCT(100), 300);
    layer2_style(parent);

    lv_obj_t *list = lv_obj_create(parent);
    lv_obj_set_size(list, 220, 220);
    lv_obj_set_align(list, LV_ALIGN_CENTER);

    lv_obj_t *act = lv_animimg_create(list);
    lv_obj_center(act);
    lv_animimg_set_src(act, (const void **)anim_imgs, 2);
    lv_animimg_set_duration(act, 5000);
    lv_animimg_set_repeat_count(act, LV_ANIM_REPEAT_INFINITE); // 無限重複
    lv_animimg_start(act);

    /*****第二層 選單 對其 menu**** */

    lv_obj_align_to(parent, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

static void anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void anim_size_cb(void *var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

static lv_anim_t a1;
static lv_anim_t a2;

static void btn_start_event_handler(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    lv_anim_timeline_t *anim_timeline = lv_event_get_user_data(e);

    bool reverse = lv_obj_has_state(btn, LV_STATE_CHECKED);
    lv_anim_timeline_set_reverse(anim_timeline, reverse);

    lv_anim_timeline_start(anim_timeline);
}

void test4(lv_obj_t *parent)
{
    /* Create anim timeline */
    lv_anim_timeline_t *anim_timeline = lv_anim_timeline_create();

    lv_obj_t *par = lv_obj_create(parent);
    lv_obj_set_size(par, LV_PCT(100), LV_PCT(100));

    /* create btn_start */
    lv_obj_t *btn_start = lv_btn_create(par);
    lv_obj_add_event_cb(btn_start, btn_start_event_handler, LV_EVENT_VALUE_CHANGED, anim_timeline);
    lv_obj_add_flag(btn_start, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_add_flag(btn_start, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t *label_start = lv_label_create(btn_start);
    lv_label_set_text(label_start, "Start");
    lv_obj_center(label_start);

    /* create 3 objects */
    lv_obj_t *obj1 = lv_obj_create(par);
    lv_obj_set_size(obj1, 60, 60);
    lv_obj_set_scrollbar_mode(obj1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(obj1, LV_ALIGN_TOP_MID, 0, 0);

    /* anim-obj1 */
    lv_anim_init(&a1);
    lv_anim_set_var(&a1, obj1);
    lv_anim_set_values(&a1, 10, 200);
    lv_anim_set_path_cb(&a1, lv_anim_path_linear);
    lv_anim_set_time(&a1, 300);
    lv_anim_set_exec_cb(&a1, anim_y_cb);

    lv_anim_init(&a2);
    lv_anim_set_var(&a2, obj1);
    lv_anim_set_values(&a2, 10, 200);
    lv_anim_set_path_cb(&a2, lv_anim_path_linear);
    lv_anim_set_time(&a2, 300);
    lv_anim_set_exec_cb(&a2, anim_y_cb);

    /* add animations to timeline */
    lv_anim_timeline_add(anim_timeline, 100, &a1);
    lv_anim_timeline_add(anim_timeline, 100, &a2);
}

const void *game_img[3] = {&menu, &Activity, &home};

static void lottery_page()
{
    /*****第一層 背景**** */
    lottery_page_bg = lv_obj_create(NULL);
    bg_pic(lottery_page_bg);

    /*****第二層 **** */

    parent0 = lv_obj_create(lottery_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    // lv_obj_t *cont_row = lv_obj_create(parent0);
    // lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    // lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    // lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    // lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    // layer2_style(cont_row);

    // home_btn = lv_btn_create(cont_row);
    // btn_pic(home_btn, &home);

    // Activity_btn = lv_btn_create(cont_row);
    // btn_pic(Activity_btn, &Activity);

    // menu_btn = lv_btn_create(cont_row);
    // btn_pic(menu_btn, &menu);

    layer2_style(parent0);

    /*****第二層 遊戲**** */

    test4(parent0);

    /*****第二層 選單 對其 menu**** */

    // lv_obj_align_to(parent, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

typedef enum
{
    SET_MENU_ITEM1,
    SET_MENU_ITEM2,
} set_menu_item;

static lv_obj_t *slider_label;

static lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt,
                             set_menu_item set_item)
{
    lv_obj_t *obj = lv_menu_cont_create(parent);

    lv_obj_t *img = NULL;
    lv_obj_t *label = NULL;

    if (icon)
    {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if (txt)
    {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if (set_item == SET_MENU_ITEM2 && icon && txt)
    {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);

    int slider_value = lv_slider_get_value(slider);

    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", slider_value);
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    tplayer_setvolume(slider_value);
    LV_LOG_USER("slider_value:%d\n", slider_value);

    volume = tplayer_getvolume();
    LV_LOG_USER("volume:%d\n", volume);
}

static lv_obj_t *create_slider(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max, int val)
{
    lv_obj_t *obj = create_text(parent, icon, txt, SET_MENU_ITEM2);

    lv_obj_t *slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_obj_set_style_anim_duration(slider, 2000, 0);

    /*****Create a label below the slider*****/

    slider_label = lv_label_create(obj);
    lv_label_set_text(slider_label, "0%");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    if (icon == NULL)
    {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

void set_list(lv_obj_t *parent)
{
    volume = tplayer_getvolume();

    /*****Create a menu object*****/
    lv_obj_t *set_menu = lv_menu_create(parent);
    lv_obj_set_size(set_menu, LV_PCT(100), LV_PCT(100));
    lv_obj_center(set_menu);

    /*****Modify the header*****/
    lv_obj_t *back_btn = lv_menu_get_main_header_back_button(set_menu);
    lv_obj_t *back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label, "Back");

    lv_obj_t *cont;
    // lv_obj_t *label;
    // lv_obj_t *section;

    /*****Create sub pages*****/
    lv_obj_t *sub_1_page = lv_menu_page_create(set_menu, NULL);
    cont = create_slider(sub_1_page, LV_SYMBOL_AUDIO, "volume", 0, 100, 50);
    // cont = create_slider(sub_1_page, LV_SYMBOL_AUDIO, "volume", 0, 100, volume);

    /*****Create a main page*****/
    lv_obj_t *main_page = lv_menu_page_create(set_menu, NULL);
    cont = create_text(main_page, LV_SYMBOL_SETTINGS, "volume", SET_MENU_ITEM1);

    lv_menu_set_load_page_event(set_menu, cont, sub_1_page);

    lv_menu_set_page(set_menu, main_page);
}

static void setting_page()
{
    /*****第一層 背景**** */
    set_page_bg = lv_obj_create(NULL);
    bg_pic(set_page_bg);

    /*****第二層 **** */

    parent0 = lv_obj_create(set_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    home_btn = lv_btn_create(cont_row);
    btn_pic(home_btn, &home, event_handler);

    Activity_btn = lv_btn_create(cont_row);
    btn_pic(Activity_btn, &Activity, event_handler);

    menu_btn = lv_btn_create(cont_row);
    btn_pic(menu_btn, &menu, event_handler);

    layer2_style(parent0);

    /*****第二層 遊戲**** */

    lv_obj_t *parent = lv_obj_create(parent0);
    lv_obj_set_size(parent, LV_PCT(100), LV_PCT(100));
    layer2_style(parent);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);

    lv_obj_t *list = lv_obj_create(parent);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(list, 1);
    // lv_obj_add_style(list, &bg_opa_100, LV_PART_MAIN); // 矩形背景部分

    set_list(list);
    /*****第二層 選單 對其 menu**** */

    lv_obj_align_to(parent, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

void page_loop2()
{
    LV_LOG_USER("LVGL START!!!\n");
    if (tplayer_init(TPLAYER_VIDEO_ROTATE_DEGREE_270) < 0)
    {
        LV_LOG_USER("tplayer init failed\n");
        return;
    }

    media_player(MEDIA_BACKGROUD);

    // Create video play theme
    video_cont = lv_obj_create(NULL);
    lv_obj_set_style_bg_opa(video_cont, LV_OPA_TRANSP, 0);

    video_btn = lv_obj_create(video_cont);
    lv_obj_remove_style_all(video_btn);
    lv_obj_set_size(video_btn, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(video_btn, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(video_btn, video_event_cb, LV_EVENT_ALL, NULL);

    mian_page();

    lv_scr_load(main_page_bg);
}

static void video_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
        case LV_EVENT_CLICKED:
            video_active = false;
            tplayer_stop();
            video_layer_switch_opa(false);
            break;
        case LV_EVENT_LEAVE:
            if (video_active) {
                video_active = false;
                video_layer_switch_opa(false);
            }
            else
                printf("complete audio playback\n");
            break;
        default: break;
    }
}

void video_layer_switch_opa(bool opa)
{
    /* Transparent background style */
    static lv_style_t style_scr_act;
    if (style_scr_act.prop_cnt == 0) {
        lv_style_init(&style_scr_act);
        lv_style_set_bg_opa(&style_scr_act, LV_OPA_COVER);
        lv_obj_add_style(lv_scr_act(), &style_scr_act, 0);
    }

    /* 这里根据按钮状态，切换不同风格，一种是UI能够透明看到底下的视频，一种是UI覆盖视频，视频就看不到了 */
    if (opa)
    {
        printf("Switch to transparent UI\n");
        // lv_scr_load(video_cont);
        // /* 这里切换为UI透明 */
        // // lv_label_set_text(label, "Stop");
        // /* 这里设置屏幕是透明的 */
        // lv_disp_get_default()
        // lv_disp_get_default()->driver->screen_transp = 1;
        // /* 这里设置屏幕背景是透明的 */
        // lv_disp_set_bg_opa(lv_disp_get_default(), LV_OPA_TRANSP);
        // /* 这里清空屏幕，不清空的话，可能不会生效 */
        // lv_memset_00(lv_disp_get_default()->driver->draw_buf->buf_act,
        //              lv_disp_get_default()->driver->draw_buf->size * sizeof(lv_color32_t));
        // /* 这里屏幕风格切换为透明的 */
        // lv_style_set_bg_opa(&style_scr_act, LV_OPA_TRANSP);
        // /* 通知风格变化，需要更新 */
        // lv_obj_report_style_change(&style_scr_act);

        lv_scr_load(video_cont);
        lv_obj_set_style_bg_opa(video_cont, LV_OPA_TRANSP, LV_PART_MAIN);
    }
    else
    {
        printf("Switch to opaque UI\n");
        lv_scr_load(main_page_bg);
        // /* 这里切换为UI不透明，也就是覆盖视频 */
        // // lv_label_set_text(label, "Play");
        // /* 这里设置屏幕是不透明的 */
        // lv_disp_get_default()->driver->screen_transp = 0;
        // /* 这里设置屏幕背景是不透明的 */
        // lv_disp_set_bg_opa(lv_disp_get_default(), LV_OPA_COVER);
        // /* 这里屏幕风格切换为不透明的 */
        // lv_style_set_bg_opa(&style_scr_act, LV_OPA_COVER);
        // /* 通知风格变化，需要更新 */
        // lv_obj_report_style_change(&style_scr_act);

        lv_disp_t * disp = lv_disp_get_default();
        lv_disp_set_bg_opa(disp, LV_OPA_COVER);
    }
}
