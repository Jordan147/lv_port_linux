#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

    #include "lvgl/lvgl.h"

    #define MEDIA_PATH          "/usr/share/media/"
    #define MEDIA_CLICK         MEDIA_PATH "key.wav"
    #define MEDIA_BACKGROUD     MEDIA_PATH "test.wav"
    #define MEDIA_ADVERTISEMENT MEDIA_PATH "test.mp4"

    extern const lv_font_t lv_jf_16;
    LV_FONT_DECLARE(lv_jf_16);

    void page_loop2(void);
    void btn_pic(lv_obj_t *btn, const void *img_src, lv_event_cb_t event_cb);
    void play_mp4_by_aplay(const char *filename);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif