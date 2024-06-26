#ifndef LVGL_DISPLAY_H
#define LVGL_DISPLAY_H

// Includes
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "constants.h"
#include "config.hpp"
#include "touch.hpp"

// Function declarations
void init_display(Configuration config);
lv_disp_t *register_display();
void turn_off_display();
void turn_on_display();
void reset_display_off_timer();
void check_display_timeout();
void display_handle_single_touch();
void display_handle_double_touch();

#endif // LVGL_DISPLAY_H
