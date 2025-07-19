/**
 * @file fbdev.c
 *
 * Legacy framebuffer device
 *
 * Based on the original file from the repository
 *
 * Move to a separate file
 * 2025 EDGEMTech Ltd.
 *
 * Author: EDGEMTech Ltd, Erik Tagirov (erik.tagirov@edgemtech.ch)
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "src/middle_ware/gpiolib.h"
#include "lvgl/lvgl.h"
#if LV_USE_LINUX_FBDEV
#include "../simulator_util.h"
#include "../backends.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_display_t *init_fbdev(void);
static void run_loop_fbdev(void);

/**********************
 *  STATIC VARIABLES
 **********************/

static char *backend_name = "FBDEV";

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Register the backend
 *
 * @param backend the backend descriptor
 * @description configures the descriptor
 */
int backend_init_fbdev(backend_t *backend)
{
    LV_ASSERT_NULL(backend);

    backend->handle->display = malloc(sizeof(display_backend_t));
    LV_ASSERT_NULL(backend->handle->display);

    backend->handle->display->init_display = init_fbdev;
    backend->handle->display->run_loop = run_loop_fbdev;
    backend->name = backend_name;
    backend->type = BACKEND_DISPLAY;

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the fbdev driver
 *
 * @return the LVGL display
 */
static lv_display_t *init_fbdev(void)
{
    const char *device = getenv_default("LV_LINUX_FBDEV_DEVICE", "/dev/fb0");
    lv_display_t *disp = lv_linux_fbdev_create();

    if (disp == NULL) {
        return NULL;
    }

    lv_linux_fbdev_set_file(disp, device);

    return disp;
}

#define PIN_NO(port, line)      (((port) - 'A') * 0x20 + (line))
#define GPIO_KEY_PIN            PIN_NO('G', 8)
#define GPIO_LED_PIN            PIN_NO('G', 7)
#define KEY_DEBOUNCE_INTERVAL   50      // ms
#define TICK_PERIOD             5       // ms
#define LED_FLASH_INTERVAL      1000    // ms
/**
 * The run loop of the fbdev driver
 */
static void run_loop_fbdev(void)
{
    uint32_t idle_time;
    static int led_flash_count = 0;
    static bool led_state = false;


    /* Handle LVGL tasks */
    while (true) {

        /* Returns the time to the next timer execution */
        idle_time = lv_timer_handler();
        usleep(idle_time * 1000);

        if (!gpio_read(GPIO_KEY_PIN)) {
            printf("Leave ezkit\n");
            gpio_unexport(GPIO_LED_PIN);
            gpio_unexport(GPIO_KEY_PIN);
            lv_deinit();
            exit(0);  
        }

        led_flash_count++;
        if (led_flash_count >= LED_FLASH_INTERVAL / TICK_PERIOD) {
            led_flash_count = 0;
            led_state = !led_state;
            gpio_write(GPIO_LED_PIN, led_state);
        }
    }
}

#endif /*LV_USE_LINUX_FBDEV*/
