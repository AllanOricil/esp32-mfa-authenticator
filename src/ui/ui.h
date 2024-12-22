#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "ui_events.h"

	extern lv_obj_t *ui_totp_screen;
	extern lv_obj_t *ui_pin_screen;
	extern lv_obj_t *ui_touch_calibration_screen;
	extern lv_obj_t *ui_pin_textarea;
	extern lv_obj_t *ui____initial_actions0;
	extern lv_obj_t *ui_touch_calibration_screen_label;
	extern lv_obj_t *ui_touch_calibration_screen_dot_top_left;
	extern lv_obj_t *ui_touch_calibration_screen_dot_bottom_right;
	extern int _number_of_wrong_unlock_attempts;
	extern int _max_number_of_wrong_unlock_attempts;
	extern bool _display_pin_screen;
	void ui_event_totp_component_label(lv_event_t *e);
	void ui_event_totp_component_bar(lv_event_t *e);
	void ui_event_keyboard_button(lv_event_t *e);
	void ui_event_pin_textarea(lv_event_t *e);
	void init_ui(bool display_pin_screen,
				 int max_number_of_wrong_unlock_attempts);
	void load_first_screen();
	void ui_task_handler();
	void refresh_totp_labels();
	void refresh_totp_countdowns();
	void ui_touch_calibration_screen_step_2();
	void ui_touch_calibration_screen_step_3();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#ifdef __cplusplus
#include "touch.h"
#endif

#endif // UI_H
