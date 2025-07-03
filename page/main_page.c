
/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lvgl/demos/lv_demos.h"
#include "main_page.h"
#include "order_checout.h"
#include "menu_page.h"
#include "src/middle_ware/player_int.h"

/*********************
 *      DEFINES
 *********************/

extern const lv_font_t lv_jf_16;
LV_FONT_DECLARE(lv_jf_16);

static lv_obj_t *parent0;
static lv_obj_t *bg_img;

static lv_obj_t *main_page_bg;
static lv_obj_t *Activity_page_bg;
static lv_obj_t *lottery_page_bg;
static lv_obj_t *menu_page_bg;
static lv_obj_t *bill_page_bg;

static lv_obj_t *gird_cont;

static lv_obj_t *origin_btn;
static lv_obj_t *Activity_btn;
static lv_obj_t *menu_btn;
static lv_obj_t *lottery_btn;
static lv_obj_t *home_btn;

LV_IMAGE_DECLARE(bakp);
LV_IMAGE_DECLARE(home);
LV_IMAGE_DECLARE(Activity);
LV_IMAGE_DECLARE(menu);
LV_IMAGE_DECLARE(origin);
LV_IMAGE_DECLARE(lottery);
LV_IMAGE_DECLARE(act1);
LV_IMAGE_DECLARE(act2);

#define FILE_MP3 "/mnt/UDISK/music/test.mp3"
#define FILE_WAV "/mnt/UDISK/music/test.wav"
#define FILE_MP4 "/mnt/UDISK/video/test.mp4"

/**********************
 *  STATIC VARIABLES
 **********************/

static void mian_page();
static void Activity_page();
static void menu_page();
static void lottery_page();

void play_wav_by_aplay(const char *filename)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "aplay '%s' &", filename); // 加 & 可背景播放
    system(cmd);
}

void play_mp4_by_aplay(const char *filename)
{
    // char cmd[256];
    // snprintf(cmd, sizeof(cmd), "player -l -r270 '%s'", filename);
    // system(cmd);
    if (tplayer_play_url(filename) < 0) {
        printf("Failed to play video: %s\n", filename);
    } else {
        printf("Playing video: %s\n", filename);
        tplayer_play();
    }
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn_tar = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        play_wav_by_aplay("/mnt/UDISK/key.WAV");

        if (btn_tar == origin_btn)
        {
            play_mp4_by_aplay("/mnt/UDISK/test.mp4");
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
    }
}

static lv_style_t btn_style;
void btn_sty(int32_t w, int32_t h, lv_obj_t *btn)
{
    lv_obj_set_size(btn, w, h);
    lv_style_init(&btn_style);
    lv_style_set_bg_opa(&btn_style, 0);
    lv_style_set_shadow_width(&btn_style, 0);
    lv_obj_add_style(btn, &btn_style, 0);
}

void btn_pic_init(lv_obj_t *btn, const void *img_src)
{
    lv_obj_set_style_bg_image_src(btn, img_src, 0);
    btn_sty(60, 60, btn);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_PRESSED, 0);
}

static void main_btn_pic(lv_obj_t *btn, const void *img_src, int32_t cont_c, int32_t cont_r, int32_t cont_c_s, int32_t cont_r_s)
{
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, cont_c, cont_c_s, LV_GRID_ALIGN_CENTER, cont_r, cont_r_s);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_PRESSED, 0);

    lv_obj_set_style_bg_image_src(btn, img_src, 0);
    btn_sty(60, 60, btn);
}

static void main_btn()
{
    origin_btn = lv_button_create(gird_cont);
    main_btn_pic(origin_btn, &origin, 0, 0, 1, 1);

    Activity_btn = lv_button_create(gird_cont);
    main_btn_pic(Activity_btn, &Activity, 1, 1, 1, 1);

    menu_btn = lv_button_create(gird_cont);
    main_btn_pic(menu_btn, &menu, 0, 2, 1, 1);

    lottery_btn = lv_button_create(gird_cont);
    main_btn_pic(lottery_btn, &lottery, 1, 3, 1, 1);
}

static void bg_pic(lv_obj_t *bg_obj)
{
    bg_img = lv_image_create(bg_obj);
    lv_image_set_src(bg_img, &bakp);
    lv_obj_center(bg_img);
}

static lv_style_t layer2_s;
void layer2_style(lv_obj_t *obj)
{
    lv_style_init(&layer2_s);
    lv_style_set_bg_opa(&layer2_s, 0);
    lv_style_set_pad_all(&layer2_s, 1);
    lv_obj_add_style(obj, &layer2_s, 0);
}

static void mian_page()
{
    main_page_bg = lv_obj_create(NULL);
    bg_pic(main_page_bg);

    /*******網格分割******/
    static int32_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {100, 100, 100, 100, LV_GRID_TEMPLATE_LAST};

    gird_cont = lv_obj_create(main_page_bg);
    lv_obj_set_grid_align(gird_cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_dsc_array(gird_cont, col_dsc, row_dsc);
    lv_obj_set_size(gird_cont, 272, 480);
    lv_obj_set_style_bg_opa(gird_cont, 0, 0);
    lv_obj_center(gird_cont);
    lv_obj_set_layout(gird_cont, LV_LAYOUT_GRID);

    main_btn();
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

    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_btn, &home);

    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_btn, &Activity);

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

static const lv_image_dsc_t *anim_imgs[2] = {
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

    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_btn, &home);

    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_btn, &Activity);

    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_btn, &menu);

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

int lottery_img_y = 74;
int dur_count = 0;

static lv_obj_t *game_btn;
static lv_obj_t *game_btn1;
static lv_obj_t *game_btn2;
static lv_obj_t *game_img0;
static lv_obj_t *game_img1;
static lv_obj_t *game_img2;
int frame_interval;
const void *game_img[3] = {&menu, &Activity, &home};

static void lottery_pic(int frame, lv_obj_t *btn, lv_obj_t *img, int img_index)
{
    frame_interval = 420 / frame; // frame 一秒多少張，frame_interval 每張之間的間隔
    lv_image_set_src(img, game_img[img_index]);
    lv_obj_set_size(btn, 120, 120);
    lv_obj_center(img);
    lv_obj_set_pos(btn, 76, lottery_img_y);
    lottery_img_y += frame_interval;

    dur_count = lottery_img_y;

    if (lottery_img_y >= 480)
        lottery_img_y = 74;
}

static void lottery_pic2(int frame, lv_obj_t *btn, lv_obj_t *img, int img_index)
{
    frame_interval = 420 / frame; // frame 一秒多少張，frame_interval 每張之間的間隔
    lv_image_set_src(img, game_img[img_index]);
    lv_obj_set_size(btn, 120, 120);
    lv_obj_center(img);
    lv_obj_set_pos(btn, 76, lottery_img_y);
    lottery_img_y += frame_interval;

    if (lottery_img_y >= 480)
        lottery_img_y = 74;
}

static void my_timer1(lv_timer_t *timer)
{
    lottery_pic(30, game_btn, game_img0, 0);

    if (dur_count >= 200)
        lottery_pic(30, game_btn1, game_img1, 1);
}

static void lottery_page()
{
    /*****第一層 背景**** */
    lottery_page_bg = lv_obj_create(NULL);
    bg_pic(lottery_page_bg);

    /*****第二層 **** */

    parent0 = lv_obj_create(lottery_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_btn, &home);

    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_btn, &Activity);

    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_btn, &menu);

    layer2_style(parent0);

    /*****第二層 遊戲**** */

    game_btn = lv_btn_create(parent0);
    game_btn1 = lv_btn_create(parent0);
    game_btn2 = lv_btn_create(parent0);
    game_img0 = lv_image_create(game_btn);
    game_img1 = lv_image_create(game_btn1);
    game_img2 = lv_image_create(game_btn2);

    lv_timer_t *timer1;

    timer1 = lv_timer_create(my_timer1, 100, 0);
}

void page_loop2(void)
{
    play_wav_by_aplay("/mnt/UDISK/test.wav");

    mian_page();

    lv_scr_load(main_page_bg);
}
