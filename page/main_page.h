#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl/lvgl.h"

#define CEDARC_DEBUG    0

    extern const lv_font_t lv_jf_16;
    LV_FONT_DECLARE(lv_jf_16);

    void page_loop2(void);
    void btn_pic(lv_obj_t *btn, const void *img_src, lv_event_cb_t event_cb);
    void play_wav_by_aplay(const char *filename);
    void play_mp4_by_aplay(const char *filename);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif