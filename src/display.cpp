#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <string.h>
#include "constants.h"

TFT_eSPI tft = TFT_eSPI();
static lv_disp_drv_t disp_drv;

void on_display_change(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            uint32_t buffer_pos = (y - area->y1) * w + (x - area->x1);
            uint16_t color = color_p[buffer_pos].full;
            tft.writeColor(color, 1);
        }
    }

    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void init_display()
{
    pinMode(TFT_BCKL, OUTPUT);
    digitalWrite(TFT_BCKL, HIGH);

    Serial.println("Backlight enabled.");

    lv_init();
    Serial.println("LVGL initialized.");

    tft.begin();
    tft.setSwapBytes(true);
    Serial.println("TFT initialized.");
    tft.setRotation(2); /* Landscape orientation */
    Serial.println("TFT rotation set.");

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = on_display_change;
    disp_drv.draw_buf = (lv_disp_draw_buf_t *)malloc(sizeof(lv_disp_draw_buf_t));
    disp_drv.antialiasing = 1;
    void *drawBuffer = heap_caps_malloc(sizeof(lv_color_t) * LVGL_BUFFER_PIXELS, LVGL_BUFFER_MALLOC_FLAGS);
    lv_disp_draw_buf_init(disp_drv.draw_buf, drawBuffer, NULL, LVGL_BUFFER_PIXELS);
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    Serial.println("LVGL display driver registered.");
    lv_obj_clean(lv_scr_act());

    ledcSetup(PWM_CHANNEL_BCKL, PWM_FREQ_BCKL, PWM_BITS_BCKL);
    ledcAttachPin(21, PWM_CHANNEL_BCKL);
    ledcWrite(PWM_CHANNEL_BCKL, 0.5 * PWM_MAX_BCKL);
}
