#include "touch.h"

static bool _force_calibration = false;
TouchCallback _single_touch_handler;
TouchCallback _double_touch_handler;
static uint32_t first_touch_time = 0;
static bool is_double_touch = false;
static bool is_touched = false;
static int touch_counter = 0;

struct Calibration cal = {
    .xMin = 0,
    .xMax = 0,
    .yMin = 0,
    .yMax = 0};

void init_touch(
    bool force_calibration,
    TouchCallback single_touch_handler,
    TouchCallback double_touch_handler)
{
    Serial.println("Initializing touch.");

    _force_calibration = force_calibration;
    _single_touch_handler = single_touch_handler;
    _double_touch_handler = double_touch_handler;

    pinMode(TOUCH_MOSI, OUTPUT);
    pinMode(TOUCH_MISO, INPUT);
    pinMode(TOUCH_CLK, OUTPUT);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    digitalWrite(TOUCH_CLK, LOW);

    Serial.println("Touch initialized.");
}

bool touch_is_not_calibrated()
{
    return (!SPIFFS.begin(true) || (!touch_load_calibration() || _force_calibration));
}

int read_SPI(byte command)
{
    int result = 0;

    for (int i = 7; i >= 0; i--)
    {
        digitalWrite(TOUCH_MOSI, command & (1 << i));
        digitalWrite(TOUCH_CLK, HIGH);
        delayMicroseconds(10);
        digitalWrite(TOUCH_CLK, LOW);
        delayMicroseconds(10);
    }

    for (int i = 11; i >= 0; i--)
    {
        digitalWrite(TOUCH_CLK, HIGH);
        delayMicroseconds(10);
        result |= (digitalRead(TOUCH_MISO) << i);
        digitalWrite(TOUCH_CLK, LOW);
        delayMicroseconds(10);
    }

    return result;
}

void touch_calibrate_min()
{
    Serial.println("Touch the top-left corner, hold it down until the next message...");
    delay(3000);
    digitalWrite(TOUCH_CS, LOW);
    cal.xMin = read_SPI(CMD_READ_X);
    cal.yMin = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

void touch_calibrate_max()
{
    Serial.println("Touch the bottom-right corner, hold it down until the next message...");
    delay(3000);
    digitalWrite(TOUCH_CS, LOW);
    cal.xMax = read_SPI(CMD_READ_X);
    cal.yMax = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

void touch_calibrate()
{
    Serial.println("Calibration starting...");
    touch_calibrate_min();
    touch_calibrate_max();
    Serial.println("Calibration done!");
}

bool touch_load_calibration()
{
    File calFile = SPIFFS.open("/calxpt2040.txt", "r");
    if (!calFile)
    {
        return false;
    }
    cal.xMin = calFile.parseInt();
    cal.yMin = calFile.parseInt();
    cal.xMax = calFile.parseInt();
    cal.yMax = calFile.parseInt();
    calFile.close();
    return true;
}

void touch_save_calibration()
{
    File calFile = SPIFFS.open("/calxpt2040.txt", "w");
    if (!calFile)
    {
        return;
    }
    calFile.println(cal.xMin);
    calFile.println(cal.yMin);
    calFile.println(cal.xMax);
    calFile.println(cal.yMax);
    calFile.flush();
    calFile.close();
}

struct Point touch_get_touch()
{
    digitalWrite(TOUCH_CS, LOW);
    int x = read_SPI(CMD_READ_X);
    int y = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
    x = map(x, cal.xMin, cal.xMax, 0, TFT_WIDTH);
    y = map(y, cal.yMin, cal.yMax, 0, TFT_HEIGHT);
    if (x > TFT_WIDTH)
    {
        x = TFT_WIDTH;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y > TFT_HEIGHT)
    {
        y = TFT_HEIGHT;
    }
    if (y < 0)
    {
        y = 0;
    }
    return Point{x, y};
}

void touch_change_handler(lv_indev_drv_t *touchDriver, lv_indev_data_t *touchData)
{
    Point touchPoint = touch_get_touch();
    int16_t tmpTouchPointX = touchPoint.x;
    int16_t tmpTouchPointY = touchPoint.y;

    // NOTE: swapping due to the rotation
    touchPoint.x = tmpTouchPointY;
    touchPoint.y = tmpTouchPointX;

    if (touchPoint.x >= 0 && touchPoint.x < DISPLAY_WIDTH && touchPoint.y >= 0 && touchPoint.y < DISPLAY_HEIGHT)
    {
        is_touched = true;
        touchData->state = LV_INDEV_STATE_PRESSED;
        touchData->point.x = touchPoint.x;
        touchData->point.y = touchPoint.y;
    }
    else
    {
        touchData->state = LV_INDEV_STATE_RELEASED;
        if (is_touched)
        {
            uint32_t capturedTouchReleaseTime = millis();

            if (touch_counter == 1)
            {
                if (capturedTouchReleaseTime - first_touch_time <= TOUCH_DOUBLE_TOUCH_INTERVAL)
                {
                    is_double_touch = true;
                }
                else
                {
                    touch_counter--;
                }
            }

            Serial.println("RELEASED");
            touch_counter++;
            Serial.printf("TOUCH COUNTER: %d\n", touch_counter);

            if (touch_counter == 1)
            {
                first_touch_time = capturedTouchReleaseTime;
            }

            is_touched = false;
        }
    }

    if (touch_counter == 1 && millis() - first_touch_time > TOUCH_DOUBLE_TOUCH_INTERVAL)
    {
        _single_touch_handler();
        touch_counter = 0;
    }

    if (is_double_touch)
    {
        _double_touch_handler();
        is_double_touch = false;
        touch_counter = 0;
    }
}

void touch_register(lv_disp_t *disp)
{
    Serial.println("Registering touch in lvgl.");
    lv_indev_drv_t touchDriver;
    lv_indev_drv_init(&touchDriver);
    touchDriver.disp = disp;
    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = touch_change_handler;
    lv_indev_drv_register(&touchDriver);
    Serial.println("Touch registered.");
}