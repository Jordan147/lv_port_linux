#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl/demos/lv_demos.h"

#define CEDARC_DEBUG    0

void page_loop2(void);
void btn_pic_init(lv_obj_t *btn, const void *img_src);
void btn_sty(int32_t w, int32_t h, lv_obj_t * btn);
void play_wav_by_aplay(const char * filename);
void play_mp4_by_aplay(const char * filename);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif