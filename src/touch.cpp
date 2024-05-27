#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include <string.h>
#include "constants.h"

XPT2046_Bitbang touchscreen(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);
static lv_indev_drv_t indev_drv;
static uint32_t lastTouchTime = 0;
static bool isScreenOn = true;
static bool wasReleased = true;

void on_touch_change(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    Point touchPoint = touchscreen.getTouch();
    int16_t tmp_x = touchPoint.x;
    int16_t tmp_y = touchPoint.y;

    // NOTE: swapping due to the rotation
    touchPoint.x = tmp_y;
    touchPoint.y = tmp_x;

    if (touchPoint.x >= 0 && touchPoint.x < LCD_WIDTH && touchPoint.y >= 0 && touchPoint.y < LCD_HEIGHT)
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchPoint.x;
        data->point.y = touchPoint.y;

        uint32_t currentTouchTime = millis();

        lv_obj_t *active_screen = lv_scr_act();
        const char *active_screen_name = (const char *)lv_obj_get_user_data(active_screen);

        if (strcmp(active_screen_name, TOTP_SCREEN_NAME) == 0)
        {
            if (wasReleased && currentTouchTime - lastTouchTime < DOUBLE_TOUCH_INTERVAL)
            {
                if (isScreenOn)
                {
                    ledcWrite(PWM_CHANNEL_BCKL, 0);
                    isScreenOn = false;
                }
                else
                {
                    ledcWrite(PWM_CHANNEL_BCKL, 0.5 * PWM_MAX_BCKL);
                    isScreenOn = true;
                }
            }
        }

        lastTouchTime = currentTouchTime;
        wasReleased = false;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
        wasReleased = true;
    }
}

void init_touch()
{
    Serial.println("Initializing touch");
    // NOTE: these values are for xpt2040 when in landscape mode / rotated 90
    touchscreen.begin();
    touchscreen.setCalibration(178, 132, 1869, 1784);

    lv_indev_drv_init(&indev_drv);
    indev_drv.disp = lv_disp_get_default();
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = on_touch_change;
    lv_indev_drv_register(&indev_drv); // registering device driver
    Serial.println("Touch driver initialized and registered.");
}