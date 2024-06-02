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

    // NOTE: swapping due to the rotation
    touchPoint.x = tmpTouchPointY;
    touchPoint.y = tmpTouchPointX;

    if (touchPoint.x >= 0 && touchPoint.x < DISPLAY_WIDTH && touchPoint.y >= 0 && touchPoint.y < DISPLAY_HEIGHT)
    {
        uint32_t secondTouchTime = millis();

        touchData->state = LV_INDEV_STATE_PRESSED;
        touchData->point.x = touchPoint.x;
        touchData->point.y = touchPoint.y;

        lv_obj_t *activeDisplay = lv_scr_act();
        const char *activeDisplayName = (const char *)lv_obj_get_user_data(activeDisplay);
        if (strcmp(activeDisplayName, TOTP_SCREEN_NAME) == 0)
        {
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
    }
}

void calibrate()
{
    Serial.println("Calibrating touch.");
    pinMode(TFT_BCKL, OUTPUT);
    digitalWrite(TFT_BCKL, HIGH);
    delay(2000);
    touch.calibrate();
    touch.saveCalibration();
    Serial.println("Touch calibrated.");
}

void init_touch(Configuration config)
{
    Serial.println("Initializing touch.");
    touch.begin();
    if (!touch.loadCalibration() || config.touch.forceCalibration)
    {
        calibrate();
    }
    Serial.println("Touch initialized.");
}

void register_touch(lv_disp_t *disp)
{
    Serial.println("Registering touch in lvgl.");
    lv_indev_drv_init(&touchDriver);
    touchDriver.disp = disp;
    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = on_touch_change;
    lv_indev_drv_register(&touchDriver);
    Serial.println("Touch registered.");
}