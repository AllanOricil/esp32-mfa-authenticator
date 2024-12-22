#include "touch.h"

static lv_indev_drv_t touchDriver;
TouchCallback _single_touch_handler;
TouchCallback _double_touch_handler;
static uint32_t first_touch_time = 0;
static bool is_double_touch = false;
static bool is_touched = false;
static int touch_counter = 0;
static bool is_calibrated = false;

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

    _single_touch_handler = single_touch_handler;
    _double_touch_handler = double_touch_handler;

    pinMode(TOUCH_MOSI, OUTPUT);
    pinMode(TOUCH_MISO, INPUT);
    pinMode(TOUCH_CLK, OUTPUT);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    digitalWrite(TOUCH_CLK, LOW);

    if ((!SPIFFS.begin(true)) || (!touch_load_calibration() || (force_calibration)))
    {
        Serial.println("Touch must be calibrated");
        is_calibrated = false;
    }
    else
    {
        Serial.println("Touch is calibrated");
        touch_register();
        is_calibrated = true;
    }

    Serial.println("Touch initialized.");
}

bool touch_is_calibrated()
{
    return is_calibrated;
}

void touch_set_calibrated()
{
    is_calibrated = true;
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
    digitalWrite(TOUCH_CS, LOW);
    cal.xMin = read_SPI(CMD_READ_X);
    cal.yMin = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

void touch_calibrate_max()
{
    Serial.println("Touch the bottom-right corner, hold it down until the next message...");
    digitalWrite(TOUCH_CS, LOW);
    cal.xMax = read_SPI(CMD_READ_X);
    cal.yMax = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

bool touch_load_calibration()
{
    if (!SPIFFS.exists(TOUCH_CALIBRATION_SPIFFS_FILE_PATH))
    {
        Serial.println("Calibration file not found on SPIFFS!");
        return false;
    }
    File calFile = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, "r");
    if (!calFile || calFile.size() == 0)
    {
        Serial.println("File is empty or failed to open!");
        return false;
    }
    Serial.println("loading calibration");
    cal.xMin = calFile.parseInt();
    cal.yMin = calFile.parseInt();
    cal.xMax = calFile.parseInt();
    cal.yMax = calFile.parseInt();
    Serial.printf("xMin %d\n", cal.xMin);
    Serial.printf("yMin %d\n", cal.yMin);
    Serial.printf("xMax %d\n", cal.xMax);
    Serial.printf("yMax %d\n", cal.yMax);
    calFile.close();
    Serial.println("calibration loaded");
    return true;
}

void touch_save_calibration()
{
    File calFile = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, "w");
    if (!calFile)
    {
        Serial.println("File is empty or failed to open!");
        return;
    }
    calFile.println(cal.xMin);
    calFile.println(cal.yMin);
    calFile.println(cal.xMax);
    calFile.println(cal.yMax);
    calFile.flush();
    calFile.close();
    Serial.println("calibration saved");
}

struct Point touch_get_touch()
{
    digitalWrite(TOUCH_CS, LOW);
    int x = read_SPI(CMD_READ_X);
    int y = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
    // NOTE: TFT_HEIGHT and TFT_WIDTH are inverted because the touch sensor is oriented in the vertical position
    x = map(x, cal.xMin, cal.xMax, 0, TFT_HEIGHT);
    y = map(y, cal.yMin, cal.yMax, 0, TFT_WIDTH);
    if (x > TFT_HEIGHT)
    {
        x = TFT_HEIGHT;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y > TFT_WIDTH)
    {
        y = TFT_WIDTH;
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

void touch_register()
{
    Serial.println("Registering touch in lvgl.");
    lv_disp_t *disp = lv_disp_get_default();
    if (!disp)
    {
        Serial.println("Error: Default display is NULL.");
        return;
    }
    lv_indev_drv_init(&touchDriver);
    touchDriver.disp = disp;
    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = touch_change_handler;
    lv_indev_t *indev = lv_indev_drv_register(&touchDriver);
    if (!indev)
    {
        Serial.println("Error: Failed to register input driver.");
        return;
    }
    Serial.println("Touch registered.");
}