#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include <string.h>
#include "constants.h"
#include "config.hpp"
#include "touch.hpp"

TouchCallback _singleTouchCallback;
TouchCallback _doubleTouchCallback;
XPT2046_Bitbang touch(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);
static lv_indev_drv_t touchDriver;
static uint32_t firstTouchTime = 0;
static bool doubleTouch = false;
static int touchCounter = 0;
static int touched = 0;
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
        touched = 1;
        touchData->state = LV_INDEV_STATE_PRESSED;
        touchData->point.x = touchPoint.x;
        touchData->point.y = touchPoint.y;
    }
    else
    {
        touchData->state = LV_INDEV_STATE_RELEASED;
        if (touched == 1)
        {
            uint32_t capturedTouchReleaseTime = millis();

            if (touchCounter == 1)
            {
                if (capturedTouchReleaseTime - firstTouchTime <= TOUCH_DOUBLE_TOUCH_INTERVAL)
                {
                    doubleTouch = true;
                }
                else
                {
                    touchCounter--;
                }
            }

            Serial.println("RELEASED");
            touchCounter++;
            Serial.printf("TOUCH COUNTER: %d\n", touchCounter);

            if (touchCounter == 1)
            {
                firstTouchTime = capturedTouchReleaseTime;
            }

            touched = 0;
        }
    }

    if (touchCounter == 1 && millis() - firstTouchTime > TOUCH_DOUBLE_TOUCH_INTERVAL)
    {
        _singleTouchCallback();
        touchCounter = 0;
    }

    if (doubleTouch)
    {
        _doubleTouchCallback();
        doubleTouch = false;
        touchCounter = 0;
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

void init_touch(Configuration config, TouchCallback singleTouchCallback, TouchCallback doubleTouchCallback)
{
    Serial.println("Initializing touch.");
    touch.begin();
    if (!touch.loadCalibration() || config.touch.forceCalibration)
    {
        calibrate();
    }

    _singleTouchCallback = singleTouchCallback;
    _doubleTouchCallback = doubleTouchCallback;
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