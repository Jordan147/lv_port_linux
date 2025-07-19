#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl/lvgl.h"

    enum MENU_DIS_CHANGE
    {
        M_D_LIST,
        M_SD_LIST,
        M_SF_LIST,
        M_ITEM,
        M_BILL,
    };

    enum ORDER_CHECK
    {
        O_IDLE,
        O_IDLE2,
        O_NEW_ORDER,
        O_OLD_ORDER,
        O_WORKING = 100,
        O_DONE = 200,
    };

    void menu_list(lv_obj_t *parent);
    void top_menu(lv_obj_t *parent);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif