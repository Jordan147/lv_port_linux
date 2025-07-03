
/*********************
 *      INCLUDES
 *********************/
// #include "lv_100ask_lesson_demos/lv_100ask_lesson_demos.h"
#include <stdio.h>
#include <stdlib.h>

// #define CEDARC_DEBUG    0
// #define USE_EXT_FONT    1

#include "lvgl/demos/lv_demos.h"
#include "freetype/lv_freetype.h"
#include "src/middle_ware/player_int.h"
#include "main_page.h"

#include "picture/bakp.c"
#include "picture/Activity.c"
#include "picture/btn_mune.c"
#include "picture/lottery.c"
#include "picture/origin.c"
#include "picture/home.c"
#include "picture/act1.c"
#include "picture/act2.c"
#include "picture/item_100.c"

/*********************
 *      DEFINES
 *********************/
static lv_obj_t *bg_img;

static lv_obj_t *main_page_bg;
static lv_obj_t *origin_page_bg;
static lv_obj_t *Activity_page_bg;
static lv_obj_t *lottery_page_bg;
static lv_obj_t *menu_page_bg;

static lv_obj_t *cont;

static lv_obj_t *origin_img;
static lv_obj_t *Activity_img;
static lv_obj_t *menu_img;
static lv_obj_t *lottery_img;
static lv_obj_t *home_img;

static lv_obj_t *origin_btn;
static lv_obj_t *Activity_btn;
static lv_obj_t *menu_btn;
static lv_obj_t *lottery_btn;
static lv_obj_t *home_btn;
// static lv_obj_t *next_btn;
// static lv_obj_t *prev_btn;

#if USE_EXT_FONT
static const lv_font_t *font_small;
static const lv_font_t *font_large;
#endif

#define FILE_MP3 "/root/media/1-dream.mp3"
#define FILE_WAV "/root/media/2-chess.mp3"
#define FILE_MP4 "/root/media/DEMO272X480-2.mp4"

/**********************
 *  STATIC VARIABLES
 **********************/

static void mian_page(void);
static void origin_page(void);
static void Activity_page(void);
static void menu_page(void);
static void lottery_page(void);

static void play_mp4_by_aplay(const char *filename);
static void play_wav_by_aplay(const char *filename);
static void btn_sty(int32_t w, int32_t h, lv_obj_t *btn);
static void btn_pic_init(lv_obj_t *img, lv_obj_t *btn, const void *img_src);
static void main_btn_pic(lv_obj_t *img, lv_obj_t *btn, const void *img_src, int32_t cont_c, int32_t cont_r, int32_t cont_c_s, int32_t cont_r_s);
static void main_btn();
static void bg_pic(lv_obj_t *bg_obj);
void layer2_style(lv_obj_t *obj);

static void play_wav_by_aplay(const char *filename) {
    // char cmd[256];
    // snprintf(cmd, sizeof(cmd), "aplay '%s' &", filename);  // 加 & 可背景播放
    // system(cmd);
    if (tplayer_play_url(filename) < 0) {
        printf("Failed to play video: %s\n", filename);
    } else {
        printf("Playing video: %s\n", filename);
        tplayer_play();
    }
}

static void play_mp4_by_aplay(const char *filename)
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
        play_wav_by_aplay(FILE_MP3);

        if (btn_tar == origin_btn)
        {
            play_mp4_by_aplay(FILE_MP4);
            // origin_page();
            // lv_scr_load_anim(origin_page_bg, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 0, false);
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
    else if (code == LV_EVENT_PRESSED){
    }
}

static lv_style_t btn_style;
static void btn_sty(int32_t w, int32_t h, lv_obj_t *btn)
{
    lv_obj_set_size(btn, w, h);
    lv_style_init(&btn_style);
    lv_style_set_bg_opa(&btn_style, 0);
    lv_style_set_shadow_width(&btn_style, 0);
    lv_obj_add_style(btn, &btn_style, 0);
}

static void btn_pic_init(lv_obj_t *img, lv_obj_t *btn, const void *img_src)
{
    img = lv_image_create(btn);
    lv_image_set_src(img, img_src);
    lv_obj_center(img);

    btn_sty(60, 60, btn);

    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_PRESSED, 0);
}

static void main_btn_pic(lv_obj_t *img, lv_obj_t *btn, const void *img_src, int32_t cont_c, int32_t cont_r, int32_t cont_c_s, int32_t cont_r_s)
{
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, cont_c, cont_c_s, LV_GRID_ALIGN_CENTER, cont_r, cont_r_s);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(btn, event_handler,  LV_EVENT_PRESSED, 0);

    img = lv_image_create(btn);
    lv_image_set_src(img, img_src);
    lv_obj_center(img);

    btn_sty(60, 60, btn);
}

static void main_btn()
{
    origin_btn = lv_button_create(cont);
    LV_IMAGE_DECLARE(origin);
    main_btn_pic(origin_img, origin_btn, &origin, 0, 0, 1, 1);

    Activity_btn = lv_button_create(cont);
    LV_IMAGE_DECLARE(Activity);
    main_btn_pic(Activity_img, Activity_btn, &Activity, 1, 1, 1, 1);

    menu_btn = lv_button_create(cont);
    LV_IMAGE_DECLARE(btn_mune);
    main_btn_pic(menu_img, menu_btn, &btn_mune, 0, 2, 1, 1);

    lottery_btn = lv_button_create(cont);
    LV_IMAGE_DECLARE(lottery);
    main_btn_pic(lottery_img, lottery_btn, &lottery, 1, 3, 1, 1);
}

static void bg_pic(lv_obj_t *bg_obj)
{
    // lv_obj_set_size(bg_obj, LV_PCT(100), LV_PCT(100));

    LV_IMAGE_DECLARE(bakp);
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

    cont = lv_obj_create(main_page_bg);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_set_size(cont, 272, 480);
    lv_obj_set_style_bg_opa(cont, 0, 0);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    main_btn();
}

static lv_style_t style_text_muted;
static lv_obj_t *create_shop_item(lv_obj_t *parent, const void *img_src, const char *name, const char *category,
                                  const char *price)
{
    static int32_t grid_col_dsc[] = {LV_GRID_CONTENT, 5, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t grid_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_style_init(&style_text_muted);
#if USE_EXT_FONT
    lv_style_set_text_font(&style_text_muted, font_small);
#endif
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);

    cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_grid_dsc_array(cont, grid_col_dsc, grid_row_dsc);

    lv_obj_t *img = lv_image_create(cont);
    lv_image_set_src(img, img_src);
    lv_obj_set_grid_cell(img, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 2);

    lv_obj_t *label;
    label = lv_label_create(cont);
    lv_label_set_text(label, name);
    lv_obj_add_style(label, &style_text_muted, 0);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_END, 0, 1);

    label = lv_label_create(cont);
    lv_label_set_text(label, category);
    lv_obj_add_style(label, &style_text_muted, 0);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_START, 1, 1);

    label = lv_label_create(cont);
    lv_label_set_text(label, price);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 2, 1, LV_GRID_ALIGN_END, 2, 1);

    return cont;
}

static void menu_page()
{
    /*****第一層 背景**** */
    menu_page_bg = lv_obj_create(NULL);
    bg_pic(menu_page_bg);

    /*****第二層 **** */

    lv_obj_t *parent0 = lv_obj_create(menu_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    LV_IMAGE_DECLARE(home);
    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_img, home_btn, &home);

    LV_IMAGE_DECLARE(Activity);
    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_img, Activity_btn, &Activity);

    LV_IMAGE_DECLARE(btn_mune);
    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_img, menu_btn, &btn_mune);

    /*****第二層 menu**** */

    layer2_style(parent0);

    lv_obj_t *parent = lv_obj_create(parent0);
    lv_obj_set_size(parent, LV_PCT(100), LV_SIZE_CONTENT);
    layer2_style(parent);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);

    lv_obj_t *list = lv_obj_create(parent);
    // lv_obj_add_flag(list, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_height(list, 310);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(list, 1);
    // lv_obj_add_flag(list, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    LV_IMAGE_DECLARE(item_100);
#if USE_EXT_FONT
    create_shop_item(list, &item_100, "套餐", "衣服", "$722");
    create_shop_item(list, &item_100, "套餐", "衣服", "$411");
    create_shop_item(list, &item_100, "套餐", "衣服", "$917");
    create_shop_item(list, &item_100, "套餐", "衣服", "$917");
    create_shop_item(list, &item_100, "套餐", "衣服", "$917");
    create_shop_item(list, &item_100, "套餐", "衣服", "$917");
#else
    create_shop_item(list, &item_100, "set menu", "Clothes", "$722");
    create_shop_item(list, &item_100, "set menu", "Clothes", "$411");
    create_shop_item(list, &item_100, "set menu", "Clothes", "$917");
    create_shop_item(list, &item_100, "set menu", "Clothes", "$917");
    create_shop_item(list, &item_100, "set menu", "Clothes", "$917");
    create_shop_item(list, &item_100, "set menu", "Clothes", "$917");
#endif
    /*****第二層 選單 對其 menu**** */

    lv_obj_align_to(parent, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

static void origin_page()
{
    /*****第一層 背景**** */
    origin_page_bg = lv_obj_create(NULL);
    bg_pic(origin_page_bg);

    /*****第二層 **** */

    lv_obj_t *parent0 = lv_obj_create(origin_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    LV_IMAGE_DECLARE(home);
    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_img, home_btn, &home);

    LV_IMAGE_DECLARE(Activity);
    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_img, Activity_btn, &Activity);

    LV_IMAGE_DECLARE(btn_mune);
    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_img, menu_btn, &btn_mune);

    layer2_style(parent0);
}

LV_IMAGE_DECLARE(act1);
LV_IMAGE_DECLARE(act2);

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

    lv_obj_t *parent0 = lv_obj_create(Activity_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    LV_IMAGE_DECLARE(home);
    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_img, home_btn, &home);

    LV_IMAGE_DECLARE(Activity);
    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_img, Activity_btn, &Activity);

    LV_IMAGE_DECLARE(btn_mune);
    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_img, menu_btn, &btn_mune);

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

static void lottery_page()
{
    /*****第一層 背景**** */
    lottery_page_bg = lv_obj_create(NULL);
    bg_pic(lottery_page_bg);

    /*****第二層 **** */

    lv_obj_t *parent0 = lv_obj_create(lottery_page_bg);
    lv_obj_set_size(parent0, LV_PCT(100), LV_PCT(100));

    /*****第二層 選單**** */

    lv_obj_t *cont_row = lv_obj_create(parent0);
    lv_obj_set_size(cont_row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_align(cont_row, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    layer2_style(cont_row);

    LV_IMAGE_DECLARE(home);
    home_btn = lv_button_create(cont_row);
    btn_pic_init(home_img, home_btn, &home);

    LV_IMAGE_DECLARE(Activity);
    Activity_btn = lv_button_create(cont_row);
    btn_pic_init(Activity_img, Activity_btn, &Activity);

    LV_IMAGE_DECLARE(btn_mune);
    menu_btn = lv_button_create(cont_row);
    btn_pic_init(menu_img, menu_btn, &btn_mune);

    layer2_style(parent0);
}

void page_loop2(void)
{
    if (tplayer_init(TPLAYER_VIDEO_ROTATE_DEGREE_90) < 0)
    {
        printf("tplayer init failed\n");
        return;
    }

    if (tplayer_setvolume(100) < 0)
    {
        printf("set volume failed\n");
        return;
    }

#if USE_EXT_FONT
    // Initialize the FreeType font library
    if (lv_freetype_init() != LV_FREETYPE_INIT_SUCCESS) {
        LV_LOG_ERROR("freetype init failed.");
        return;
    }

    // Load the font with FreeType
    // #if 0
    //     font_small = lv_freetype_font_create("./lvgl/examples/libs/freetype/Lato-Regular.ttf",
    //                                          LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
    //                                          12,
    //                                          LV_FREETYPE_FONT_STYLE_NORMAL);
    //     font_large = lv_freetype_font_create("./lvgl/examples/libs/freetype/Lato-Regular.ttf",
    //                                          LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
    //                                          16,
    //                                          LV_FREETYPE_FONT_STYLE_NORMAL);
    // #else
    //     font_small = lv_freetype_font_create("./yayuan.ttf",
    //                                          LV_FREETYPE_FONT_RENDER_MODE_OUTLINE, //LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
    //                                          12,
    //                                          LV_FREETYPE_FONT_STYLE_NORMAL);
    //     if (font_small == NULL) {
    //         die("Failed to load font yayuan.ttf with size 12\n");
    //     }
    //     font_large = lv_freetype_font_create("./yayuan.ttf",
    //                                          LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
    //                                          16,
    //                                          LV_FREETYPE_FONT_STYLE_NORMAL);
    //     if (font_large == NULL) {
    //         die("Failed to load font yayuan.ttf with size 16\n");
    //     }
    // #endif
#if 1
    lv_font_t * font = lv_freetype_font_create("./lvgl/examples/libs/freetype/Lato-Regular.ttf",
                                               LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                               20,
                                               LV_FREETYPE_FONT_STYLE_NORMAL);
#else
    lv_font_t * font = lv_freetype_font_create("D:/100ask/freetype/SourceHanSansCN-Bold-2.otf",
                                               LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                               100,
                                               LV_FREETYPE_FONT_STYLE_NORMAL);
#endif

    if(!font) {
        LV_LOG_ERROR("freetype font create failed.");
        return;
    }

#endif

    // play_wav_by_aplay(FILE_WAV);

    mian_page();

    lv_scr_load(main_page_bg);
}
