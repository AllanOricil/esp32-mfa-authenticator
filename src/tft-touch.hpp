#ifndef LVGL_DISPLAY_H
#define LVGL_DISPLAY_H

// Includes
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Bitbang.h>

// Definitions
#define LVGL_TICK_PERIOD_MS 1
#define LVGL_BUFFER_PIXELS (LCD_WIDTH*LCD_HEIGHT/4)
#define LVGL_BUFFER_MALLOC_FLAGS (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)
#define RERUN_CALIBRATE true

// Declarations
extern TFT_eSPI tft;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
extern XPT2046_Bitbang touchscreen;

// Function declarations
void flush_disp(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void on_touchpad_change(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);
void init_display_and_touch();
void display_and_touch_handler();

#endif // LVGL_DISPLAY_H
