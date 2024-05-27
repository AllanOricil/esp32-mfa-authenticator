#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include <string.h>
#include "constants.h"

XPT2046_Bitbang touchscreen(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);
static lv_indev_drv_t touchDriver;
static uint32_t firstTouchTime = 0;
static bool isDisplayOn = true;

void on_touch_change(lv_indev_drv_t *touchDriver, lv_indev_data_t *touchData)
{
    Point touchPoint = touchscreen.getTouch();
    int16_t tmpTouchPointX = touchPoint.x;
    int16_t tmpTouchPointY = touchPoint.y;

    // NOTE: swapping due to the rotation
    touchPoint.x = tmpTouchPointY;
    touchPoint.y = tmpTouchPointX;

    if (touchPoint.x >= 0 && touchPoint.x < LCD_WIDTH && touchPoint.y >= 0 && touchPoint.y < LCD_HEIGHT)
    {
        uint32_t secondTouchTime = millis();

        touchData->state = LV_INDEV_STATE_PRESSED;
        touchData->point.x = touchPoint.x;
        touchData->point.y = touchPoint.y;

        lv_obj_t *activeDisplay = lv_scr_act();
        const char *activeDisplayName = (const char *)lv_obj_get_user_data(activeDisplay);
        if (strcmp(activeDisplayName, TOTP_SCREEN_NAME) == 0)
        {
            if (touchData->state == LV_INDEV_STATE_RELEASED && secondTouchTime - firstTouchTime < DOUBLE_TOUCH_INTERVAL)
            {
                if (isDisplayOn)
                {
                    ledcWrite(PWM_CHANNEL_BCKL, 0);
                }
                else
                {
                    ledcWrite(PWM_CHANNEL_BCKL, 0.5 * PWM_MAX_BCKL);
                }

                isDisplayOn = !isDisplayOn;
            }
        }

        firstTouchTime = secondTouchTime;
    }
    else
    {
        touchData->state = LV_INDEV_STATE_RELEASED;
    }
}

void init_touch()
{
    Serial.println("Initializing touch");
    // NOTE: these values are for xpt2040 when in landscape mode / rotated 90
    touchscreen.begin();

    // TODO: create screen to ease manual calibration
    touchscreen.setCalibration(TOUCH_X_MIN, TOUCH_Y_MIN, TOUCH_X_MAX, TOUCH_Y_MAX);

    lv_indev_drv_init(&touchDriver);
    touchDriver.disp = lv_disp_get_default();
    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = on_touch_change;
    lv_indev_drv_register(&touchDriver);
    Serial.println("Touch driver initialized and registered.");
}