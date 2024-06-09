#ifndef LVGL_TOUCH_H
#define LVGL_TOUCH_H

// Includes
#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include <string.h>
#include "constants.h"
#include "config.hpp"

typedef void (*TouchCallback)();

// Function declarations
void init_touch(Configuration config, TouchCallback singleTouchCallback, TouchCallback doubleTouchCallback);
void register_touch(lv_disp_t *disp);

#endif // LVGL_TOUCH_H
