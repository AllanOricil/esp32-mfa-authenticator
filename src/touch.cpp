#include "touch.hpp"

static lv_indev_drv_t touch_driver;
TouchCallback _single_touch_handler;
TouchCallback _double_touch_handler;
static bool is_calibrated = false;
static bool count_touch_releases = false;
static int number_touch_releases = 0;
static uint32_t first_touch_release_time = 0;
struct Calibration cal = {
    .x_min = 0,
    .x_max = 0,
    .y_min = 0,
    .y_max = 0};

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

int read_SPI(uint8_t command)
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
    cal.x_min = read_SPI(CMD_READ_X);
    cal.y_min = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

void touch_calibrate_max()
{
    Serial.println("Touch the bottom-right corner, hold it down until the next message...");
    digitalWrite(TOUCH_CS, LOW);
    cal.x_max = read_SPI(CMD_READ_X);
    cal.y_max = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
}

bool touch_load_calibration()
{
    if (!SPIFFS.exists(TOUCH_CALIBRATION_SPIFFS_FILE_PATH))
    {
        Serial.println("Calibration file not found on SPIFFS!");
        return false;
    }
    File calibration_file = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, FILE_READ);
    if (!calibration_file || calibration_file.size() == 0)
    {
        Serial.println("File is empty or failed to open!");
        return false;
    }
    Serial.println("loading calibration");
    cal.x_min = calibration_file.parseInt();
    cal.y_min = calibration_file.parseInt();
    cal.x_max = calibration_file.parseInt();
    cal.y_max = calibration_file.parseInt();
    Serial.printf("x_min %d\n", cal.x_min);
    Serial.printf("y_min %d\n", cal.y_min);
    Serial.printf("x_max %d\n", cal.x_max);
    Serial.printf("y_max %d\n", cal.y_max);
    calibration_file.close();
    Serial.println("calibration loaded");
    return true;
}

void touch_save_calibration()
{
    File calibration_file = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, FILE_WRITE);
    if (!calibration_file)
    {
        Serial.println("File is empty or failed to open!");
        return;
    }
    calibration_file.println(cal.x_min);
    calibration_file.println(cal.y_min);
    calibration_file.println(cal.x_max);
    calibration_file.println(cal.y_max);
    calibration_file.flush();
    calibration_file.close();
    Serial.println("calibration saved");
}

struct Point touch_get_touch()
{
    digitalWrite(TOUCH_CS, LOW);
    int x = read_SPI(CMD_READ_X);
    int y = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
    // NOTE: TFT_HEIGHT and TFT_WIDTH are inverted because the touch sensor is oriented in the vertical position
    x = map(x, cal.x_min, cal.x_max, 0, DISPLAY_HEIGHT);
    y = map(y, cal.y_min, cal.y_max, 0, DISPLAY_WIDTH);
    if (x > DISPLAY_HEIGHT)
    {
        x = DISPLAY_HEIGHT;
    }
    if (x < 0)
    {
        x = 0;
    }
    if (y > DISPLAY_WIDTH)
    {
        y = DISPLAY_WIDTH;
    }
    if (y < 0)
    {
        y = 0;
    }
    return Point{x, y};
}

void touch_change_handler(lv_indev_drv_t *touch_driver, lv_indev_data_t *touch_data)
{
    Point touched_point = touch_get_touch();
    int16_t tmp_touched_point_x = touched_point.x;
    int16_t tmp_touched_point_y = touched_point.y;

    // NOTE: swapping because the TFT is rotated
    touched_point.x = tmp_touched_point_y;
    touched_point.y = tmp_touched_point_x;
    if (touched_point.x >= 0 && touched_point.x < DISPLAY_WIDTH && touched_point.y >= 0 && touched_point.y < DISPLAY_HEIGHT)
    {
        count_touch_releases = true;
        touch_data->state = LV_INDEV_STATE_PRESSED;
        touch_data->point.x = touched_point.x;
        touch_data->point.y = touched_point.y;
    }
    else
    {
        touch_data->state = LV_INDEV_STATE_RELEASED;
        if (count_touch_releases)
        {
            count_touch_releases = false;
            uint32_t touch_release_time = millis();
            if (!first_touch_release_time)
            {

                first_touch_release_time = touch_release_time;
            }

            number_touch_releases++;
        }
    }

    if (millis() - first_touch_release_time > TOUCH_DOUBLE_TOUCH_INTERVAL)
    {
        if (number_touch_releases == 2)
        {
            _double_touch_handler();
        }

        if (number_touch_releases == 1)
        {
            _single_touch_handler();
        }

        number_touch_releases = 0;
        first_touch_release_time = 0;
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
    lv_indev_drv_init(&touch_driver);
    touch_driver.disp = disp;
    touch_driver.type = LV_INDEV_TYPE_POINTER;
    touch_driver.read_cb = touch_change_handler;
    lv_indev_t *indev = lv_indev_drv_register(&touch_driver);
    if (!indev)
    {
        Serial.println("Error: Failed to register input driver.");
        return;
    }
    Serial.println("Touch registered.");
}