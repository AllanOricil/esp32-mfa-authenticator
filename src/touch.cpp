#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include <string.h>
#include "constants.h"
#include "config.hpp"

XPT2046_Bitbang touch(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);
static lv_indev_drv_t touchDriver;
static uint32_t firstTouchTime = 0;
static bool isDisplayOn = true;
static bool captureSecondTouch = false;

void on_touch_change(lv_indev_drv_t *touchDriver, lv_indev_data_t *touchData)
{
    Point touchPoint = touch.getTouch();
    int16_t tmpTouchPointX = touchPoint.x;
    int16_t tmpTouchPointY = touchPoint.y;
    // Serial.printf("BEFORE TX: %d TY %d \n", touchPoint.x, touchPoint.y);

    // NOTE: swapping due to the rotation
    touchPoint.x = tmpTouchPointY;
    touchPoint.y = tmpTouchPointX;

    // Serial.printf("AFTER TX: %d TY %d \n", touchPoint.x, touchPoint.y);
    //  NOTE: because x and y cordinates were swaped, x limits are now bound to DISPLAY HEIGHT, while y limits to the DISPLAY_WIDTH
    if (touchPoint.x >= 0 && touchPoint.x < DISPLAY_HEIGHT && touchPoint.y >= 0 && touchPoint.y < DISPLAY_WIDTH)
    {
        // Serial.println("TOUCH PRESSED");
        uint32_t secondTouchTime = millis();

        touchData->state = LV_INDEV_STATE_PRESSED;
        touchData->point.x = touchPoint.x;
        touchData->point.y = touchPoint.y;

        lv_obj_t *activeDisplay = lv_scr_act();
        const char *activeDisplayName = (const char *)lv_obj_get_user_data(activeDisplay);
        if (strcmp(activeDisplayName, TOTP_SCREEN_NAME) == 0)
        {
            // Serial.printf("TIME SINCE FIRST TOUCH %lu \n", secondTouchTime - firstTouchTime);
            // Serial.printf("STATE %d \n", touchData->state);
            if (captureSecondTouch && secondTouchTime - firstTouchTime < TOUCH_DOUBLE_TOUCH_INTERVAL)
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
        captureSecondTouch = false;
    }
    else
    {
        touchData->state = LV_INDEV_STATE_RELEASED;
        captureSecondTouch = true;
        // Serial.println("TOUCH RELEASED");
    }
}

void init_touch(Configuration config)
{
    Serial.println("Initializing touch");
    // NOTE: these values are for XPT2040 when in landscape mode / rotated 90
    touch.begin();

    // TODO: create screen to enable touch calibration without using serial output
    if (config.touch.force_calibration)
    {
        Serial.println("Calibrating touch");
        delay(2000);
        touch.calibrate();
        touch.saveCalibration();
    }
    else
    {
        touch.setCalibration(TOUCH_X_MIN, TOUCH_Y_MIN, TOUCH_X_MAX, TOUCH_Y_MAX);
    }

    lv_indev_drv_init(&touchDriver);
    touchDriver.disp = lv_disp_get_default();
    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = on_touch_change;
    lv_indev_drv_register(&touchDriver);
    Serial.println("Touch initialized");
}