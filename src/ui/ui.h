// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"

	// SCREEN: ui_TotpScreen
	extern lv_obj_t *ui_totp_screen;
	extern lv_obj_t *ui_pin_screen;
	extern lv_obj_t *ui_load_screen;
	extern lv_obj_t *ui_pin_textarea;
	extern lv_obj_t *ui____initial_actions0;
	void ui_event_totp_component_label(lv_event_t *e);
	void ui_event_totp_component_bar(lv_event_t *e);
	void ui_event_keyboard_button(lv_event_t *e);
	void ui_event_pin_textarea(lv_event_t *e);
	void ui_init(void);
	void ui_task_handler();
	void refresh_totp_labels();
	void refresh_counter_bars();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
