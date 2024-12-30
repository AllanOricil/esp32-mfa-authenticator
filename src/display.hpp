#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include <Arduino.h>
#include <lvgl.h>
#include <string.h>
#include <esp_log.h>
#include "constants.h"
#include "./ui/ui.h"
#include "config.hpp"

void init_display(Configuration config);
void display_register();
void turn_off_display();
void turn_on_display();
void reset_display_off_timer();
void check_display_timeout();
void display_handle_single_touch();
void display_handle_double_touch();

#endif // DISPLAY_H
