#ifndef TOUCH_SCREEN
#define TOUCH_SCREEN

// Library Includes
#include "config.hpp"
#include "display.h"
#include "touch.h"
#include "ui/ui.h"

void init_touch_screen(Configuration config);
void display_timeout_handler();

#endif // TOUCH_SCREEN