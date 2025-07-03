#ifndef ORDER_CHECKOUT_H
#define ORDER_CHECKOUT_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/
// #include "lv_100ask_lesson_demos/lv_100ask_lesson_demos.h"
#include "lvgl/demos/lv_demos.h"
#define MAX_MENU_ITEMS 100
#define MAX_ORDER_ITEMS 100

    typedef struct
    {
        int id;

        char name[50];

        float price;

        void *img_src;

    } MenuItem;

    typedef struct
    {
        int id;

        int menu_id;

        char name[50];

        int quantity;

        float price;

        float total;

    } OrderItem;

    void page_loop2(void);
    void addMenuItem(int id, char name, float price, int index);

    extern int menuCount;
    extern int orderCount;
    extern MenuItem menu_item[MAX_MENU_ITEMS];
    extern OrderItem order_item[MAX_ORDER_ITEMS];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
