#include "touch.hpp"

static const char *TAG = "touch";

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
    bool calibrate,
    TouchCallback single_touch_handler,
    TouchCallback double_touch_handler)
{
    ESP_LOGI(TAG, "initializing touch");

    _single_touch_handler = single_touch_handler;
    _double_touch_handler = double_touch_handler;

    pinMode(TOUCH_MOSI, OUTPUT);
    pinMode(TOUCH_MISO, INPUT);
    pinMode(TOUCH_CLK, OUTPUT);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    digitalWrite(TOUCH_CLK, LOW);

    if ((!SPIFFS.begin(true)) || (!touch_load_calibration() || (calibrate)))
    {
        ESP_LOGI(TAG, "touch must be calibrated");
        is_calibrated = false;
    }
    else
    {
        ESP_LOGI(TAG, "touch is calibrated");
        touch_register();
        is_calibrated = true;
    }

    ESP_LOGI(TAG, "touch initialized");
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
    ESP_LOGI(TAG, "reading first calibration point");
    digitalWrite(TOUCH_CS, LOW);
    cal.x_min = read_SPI(CMD_READ_X);
    cal.y_min = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
    ESP_LOGI(TAG, "first calibration point read");
}

void touch_calibrate_max()
{
    ESP_LOGI(TAG, "reading second calibration point");
    digitalWrite(TOUCH_CS, LOW);
    cal.x_max = read_SPI(CMD_READ_X);
    cal.y_max = read_SPI(CMD_READ_Y);
    digitalWrite(TOUCH_CS, HIGH);
    ESP_LOGI(TAG, "second calibration point read");
}

bool touch_load_calibration()
{
    ESP_LOGD(TAG, "loading calibration data");
    if (!SPIFFS.exists(TOUCH_CALIBRATION_SPIFFS_FILE_PATH))
    {
        ESP_LOGE(TAG, "calibration file %s not found in SPIFFS", TOUCH_CALIBRATION_SPIFFS_FILE_PATH);
        return false;
    }
    File file = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, FILE_READ);
    if (!file || file.size() == 0)
    {
        ESP_LOGE(TAG, "calibration file %s is empty of failed to open", TOUCH_CALIBRATION_SPIFFS_FILE_PATH);
        return false;
    }
    cal.x_min = file.parseInt();
    cal.y_min = file.parseInt();
    cal.x_max = file.parseInt();
    cal.y_max = file.parseInt();
    ESP_LOGV(TAG, "calibration data:");
    ESP_LOGV(TAG, "x_min %d", cal.x_min);
    ESP_LOGV(TAG, "y_min %d", cal.y_min);
    ESP_LOGV(TAG, "x_max %d", cal.x_max);
    ESP_LOGV(TAG, "y_max %d", cal.y_max);
    file.close();
    ESP_LOGD(TAG, "calibration data loaded");
    return true;
}

void touch_save_calibration()
{
    ESP_LOGI(TAG, "saving calibration data");
    File file = SPIFFS.open(TOUCH_CALIBRATION_SPIFFS_FILE_PATH, FILE_WRITE);
    if (!file)
    {
        ESP_LOGE(TAG, "calibration file %s could not be opened", TOUCH_CALIBRATION_SPIFFS_FILE_PATH);
        return;
    }
    file.println(cal.x_min);
    file.println(cal.y_min);
    file.println(cal.x_max);
    file.println(cal.y_max);
    file.flush();
    file.close();
    ESP_LOGI(TAG, "calibration data saved");
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
    ESP_LOGD(TAG, "registering touch in lvgl");
    lv_disp_t *disp = lv_disp_get_default();
    if (!disp)
    {
        ESP_LOGE(TAG, "default display can not be null");
        return;
    }
    lv_indev_drv_init(&touch_driver);
    touch_driver.disp = disp;
    touch_driver.type = LV_INDEV_TYPE_POINTER;
    touch_driver.read_cb = touch_change_handler;
    lv_indev_t *indev = lv_indev_drv_register(&touch_driver);
    if (!indev)
    {
        ESP_LOGE(TAG, "failed to register touch in lvgl");
        return;
    }
    ESP_LOGD(TAG, "touch registered in lvgl");
}