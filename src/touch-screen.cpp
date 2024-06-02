#include "config.hpp"
#include "display.hpp"
#include "touch.hpp"

void init_touch_screen(Configuration config)
{
	Serial.printf("Initializing touch screen.");

	init_touch(config);

	init_display();

	lv_disp_t *disp = register_display();

	register_touch(disp);

	Serial.printf("Touch screen initialized.");
}