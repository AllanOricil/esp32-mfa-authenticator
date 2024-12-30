#ifndef TOUCH_SCREEN
#define TOUCH_SCREEN

#include <esp_log.h>
#include "config.hpp"
#include "display.hpp"
#include "touch.hpp"

void init_touch_screen(Configuration config);
void display_timeout_handler();

#endif // TOUCH_SCREEN