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
#define RERUN_CALIBRATE false
#define PWM_CHANNEL_BCKL (SOC_LEDC_CHANNEL_NUM - 1)
#define PWM_FREQ_BCKL 400
#define PWM_BITS_BCKL 8
#define PWM_MAX_BCKL ((1 << PWM_BITS_BCKL) - 1)
#define DOUBLE_CLICK_INTERVAL 500

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
