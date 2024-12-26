#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "ui_events.h"
#include <string.h>

	typedef struct
	{
		int number_of_wrong_unlock_attempts;
		int max_number_of_wrong_unlock_attempts;
		bool display_pin_screen;
	} Config;

	extern lv_obj_t *ui_totp_screen;
	extern lv_obj_t *ui_pin_screen;
	extern lv_obj_t *ui_touch_calibration_screen;
	extern lv_obj_t *ui_pin_screen_textarea;
	extern lv_obj_t *ui_touch_calibration_screen_label;
	extern lv_obj_t *ui_touch_calibration_screen_dot_top_left;
	extern lv_obj_t *ui_touch_calibration_screen_dot_bottom_right;
	extern Config config;
	void ui_event_totp_component_label(lv_event_t *e);
	void ui_event_totp_component_countdown(lv_event_t *e);
	void ui_event_totp_screen(lv_event_t *e);
	void ui_event_pin_keyboard_button(lv_event_t *e);
	void ui_event_pin_textarea(lv_event_t *e);
	void init_ui(bool display_pin_screen,
				 int max_number_of_wrong_unlock_attempts);
	void load_first_screen();
	void ui_task_handler();
	void ui_totp_screen_update_totp_labels();
	void ui_totp_screen_update_totp_countdowns();
	void ui_totp_screen_render_totp_components();
	void ui_touch_calibration_screen_step_2();
	void ui_touch_calibration_screen_step_3();
	void ui_touch_calibration_screen_destroy();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#ifdef __cplusplus

#endif

#endif // UI_H
