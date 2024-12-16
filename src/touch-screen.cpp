#include "config.hpp"
#include "display.h"
#include "touch.h"
#include "ui/ui.h"

void single_touch_handler()
{
	Serial.println("SINGLE TOUCH");
	display_handle_single_touch();
}

void double_touch_handler()
{
	Serial.println("DOUBLE TOUCH");
	display_handle_double_touch();
}

void init_touch_screen(Configuration config)
{
	Serial.printf("Initializing touch screen.");

	init_display(config);
	init_touch(
		config.touch.forceCalibration,
		single_touch_handler,
		double_touch_handler);

	init_ui(
		config.is_pin_configured(),
		config.security.maxNumberOfWrongUnlockAttempts);

	Serial.printf("Touch screen initialized.");
}

void display_timeout_handler()
{
	check_display_timeout();
}