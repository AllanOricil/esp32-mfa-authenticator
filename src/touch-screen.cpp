#include "touch-screen.hpp"

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
	Serial.println("Initializing touch screen.");

	init_display(config);
	init_touch(
		config.touch.forceCalibration,
		single_touch_handler,
		double_touch_handler);

	Serial.println("Touch screen initialized.");
}

void display_timeout_handler()
{
	check_display_timeout();
}