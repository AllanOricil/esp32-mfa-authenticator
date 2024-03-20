#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Bitbang.h>

#define LVGL_TICK_PERIOD_MS 1
#define LVGL_BUFFER_PIXELS (LCD_WIDTH*LCD_HEIGHT/4)
#define LVGL_BUFFER_MALLOC_FLAGS (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)
#define RERUN_CALIBRATE false
#define PWM_CHANNEL_BCKL (SOC_LEDC_CHANNEL_NUM - 1)
#define PWM_FREQ_BCKL 400
#define PWM_BITS_BCKL 8
#define PWM_MAX_BCKL ((1 << PWM_BITS_BCKL) - 1)
#define DOUBLE_TOUCH_INTERVAL 500

TFT_eSPI tft = TFT_eSPI();
XPT2046_Bitbang touchscreen(TOUCH_MOSI, TOUCH_MISO, TOUCH_CLK, TOUCH_CS);

static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

void on_display_change(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);

    for(int y = area->y1; y <= area->y2; y++) {
        for(int x = area->x1; x <= area->x2; x++) {
            uint32_t buffer_pos = (y - area->y1) * w + (x - area->x1);
            uint16_t color = color_p[buffer_pos].full;
            tft.writeColor(color, 1);
        }
    }

    tft.endWrite();
    lv_disp_flush_ready(disp);
}


static uint32_t lastTouchTime = 0;  // Global state, stores time of last touch event
bool isScreenOn = true;
bool wasReleased = true;  // a flag indicating whether the previous event was a release event

void on_touchpad_change(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
    Point touchPoint = touchscreen.getTouch();
    int16_t tmp_x = touchPoint.x;
    int16_t tmp_y = touchPoint.y;

    // NOTE: swapping due to the rotation
    touchPoint.x = tmp_y;
    touchPoint.y = tmp_x;

    if (touchPoint.x >= 0 && touchPoint.x < 240 && touchPoint.y >= 0 && touchPoint.y < 320) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchPoint.x;
        data->point.y = touchPoint.y;

        uint32_t currentTouchTime = millis();

        if(wasReleased && currentTouchTime - lastTouchTime < DOUBLE_TOUCH_INTERVAL) {
            // Toggle screen if the previous event was a release event and time interval conditions are met
            if(isScreenOn){
                ledcWrite(PWM_CHANNEL_BCKL, 0);
                isScreenOn = false;
            }else{
                ledcWrite(PWM_CHANNEL_BCKL, 0.5 * PWM_MAX_BCKL);
                isScreenOn = true;
            }
        }
        lastTouchTime = currentTouchTime;  // Update time of last touch event
        wasReleased = false;  // Update the flag
    } else {
        data->state = LV_INDEV_STATE_REL;
        wasReleased = true; 
    }
}

void init_display_and_touch(){
    pinMode(TFT_BCKL, OUTPUT);
    digitalWrite(TFT_BCKL, HIGH);
	
    touchscreen.begin();
    if (!touchscreen.loadCalibration()) {
        Serial.println("Failed to load calibration data from SPIFFS.");
    }
    #if RERUN_CALIBRATE
        Serial.println("Re-running calibration as requested...");
        delay(2000); //wait for user to see serial
        touchscreen.calibrate();
        touchscreen.saveCalibration();
    #endif

    Serial.println("Backlight enabled.");

    lv_init();
    Serial.println("LVGL initialized.");

    tft.begin();
    tft.setSwapBytes(true); 
    Serial.println("TFT initialized.");
    tft.setRotation(2); /* Landscape orientation */
    Serial.println("TFT rotation set.");

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = on_display_change;
    disp_drv.draw_buf = (lv_disp_draw_buf_t *)malloc(sizeof(lv_disp_draw_buf_t));
	disp_drv.antialiasing = 1;
    void *drawBuffer = heap_caps_malloc(sizeof(lv_color_t) * LVGL_BUFFER_PIXELS, LVGL_BUFFER_MALLOC_FLAGS);
    lv_disp_draw_buf_init(disp_drv.draw_buf, drawBuffer, NULL, LVGL_BUFFER_PIXELS);
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
    Serial.println("LVGL display driver registered.");
    lv_obj_clean(lv_scr_act());

	ledcSetup(PWM_CHANNEL_BCKL, PWM_FREQ_BCKL, PWM_BITS_BCKL);
  	ledcAttachPin(21, PWM_CHANNEL_BCKL);
	ledcWrite(PWM_CHANNEL_BCKL, 0.5 * PWM_MAX_BCKL);

    lv_indev_drv_init(&indev_drv);
    indev_drv.disp = disp;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = on_touchpad_change; 
    lv_indev_t * touch_indev = lv_indev_drv_register(&indev_drv); // registering device driver
    Serial.println("Touch driver initialized and registered.");
}

void display_and_touch_handler() {
    lv_task_handler();
}
