#include "lvgl/demos/lv_demos.h"
#include "order_checout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MenuItem menu_item[MAX_MENU_ITEMS];
OrderItem order_item[MAX_ORDER_ITEMS];

int menuCount = 0;
int orderCount = 0;

void placeOrder()
{
    int itemId, quantity;

    for (int i = 0; i < menuCount; i++)
    {
        if (menu_item[i].id == itemId)
        {

            order_item[orderCount].id = menu_item[i].id;

            order_item[orderCount].quantity = quantity;

            order_item[orderCount].total = menu_item[i].price * quantity;

            orderCount++;

            printf("Order placed successfully!n");

            return;
        }
    }

    printf("Item not found!n");
}

void displayOrder()
{

    printf("Order details:n");

    for (int i = 0; i < orderCount; i++)
    {

        printf("Item ID: %d, Quantity: %d, Total: $%.2fn",

               order_item[i].id, order_item[i].quantity, order_item[i].total);
    }
}
