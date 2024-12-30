#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>
#include <lvgl.h>
#include <string.h>
#include <esp_log.h>
#include "storage.hpp"
#include "constants.h"

#define CMD_READ_Y 0x90 // Command for XPT2046 to read Y position
#define CMD_READ_X 0xD0 // Command for XPT2046 to read X position

typedef void (*TouchCallback)();

struct Point
{
	int x;
	int y;
};

struct Calibration
{
	int x_min;
	int x_max;
	int y_min;
	int y_max;
};

extern struct Calibration cal;

bool touch_is_calibrated();
void touch_set_calibrated();
void touch_calibrate_min();
void touch_calibrate_max();
void touch_calibrate();
bool touch_load_calibration();
void touch_save_calibration();
struct Point touch_get_touch();
void touch_register();
void init_touch(
	bool force_calibration,
	TouchCallback single_touch_handler,
	TouchCallback double_touch_handler);

#endif // TOUCH_H
