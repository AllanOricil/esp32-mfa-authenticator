#include "config.hpp"
#include "display.hpp"
#include "touch.hpp"

void handle_single_touch_event()
{
	Serial.println("SINGLE TOUCH");
	display_handle_single_touch();
}

void handle_double_touch_event()
{
	Serial.println("DOUBLE TOUCH");
	display_handle_double_touch();
}

void init_touch_screen(Configuration config)
{
	Serial.printf("Initializing touch screen.");

	init_touch(config, handle_single_touch_event, handle_double_touch_event);

	init_display(config);

	lv_disp_t *disp = register_display();

	register_touch(disp);

	reset_display_off_timer();

	Serial.printf("Touch screen initialized.");
}

void display_timeout_handler()
{
	check_display_timeout();
}