#ifndef DISPLAY_H
#define DISPLAY_H

// Includes
#include <Arduino.h>
#include <lvgl.h>
#include <string.h>
#include "constants.h"
#include "touch.h"
#include "./ui/ui.h"

#ifdef __cplusplus
extern "C"
{ // Begin extern "C" block for C++ compatibility
#endif
  // Function declarations
	void display_register();

#ifdef __cplusplus
} // End extern "C" block

#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.hpp"

void init_display(Configuration config);
void turn_off_display();
void turn_on_display();
void reset_display_off_timer();
void check_display_timeout();
void display_handle_single_touch();
void display_handle_double_touch();
#endif

#endif // DISPLAY_H
