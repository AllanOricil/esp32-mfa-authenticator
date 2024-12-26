#include "../ui.h"
#include "constants.h"

void ui_touch_calibration_screen_init(void)
{
    ui_touch_calibration_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui_touch_calibration_screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(ui_touch_calibration_screen, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui_touch_calibration_screen, LV_OPA_COVER, LV_PART_MAIN);

    ui_touch_calibration_screen_label = lv_label_create(ui_touch_calibration_screen);
    lv_label_set_text(ui_touch_calibration_screen_label, "Touch and hold\nthe top-left corner\nuntil the next instruction appears.");
    lv_obj_set_style_text_color(ui_touch_calibration_screen_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_align(ui_touch_calibration_screen_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(ui_touch_calibration_screen_label, LV_ALIGN_CENTER, 0, 0);

    ui_touch_calibration_screen_dot_top_left = lv_obj_create(ui_touch_calibration_screen);
    lv_obj_set_size(ui_touch_calibration_screen_dot_top_left, 10, 10);
    lv_obj_set_style_radius(ui_touch_calibration_screen_dot_top_left, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_touch_calibration_screen_dot_top_left, lv_color_make(255, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_touch_calibration_screen_dot_top_left, 0, LV_PART_MAIN);
    lv_obj_align(ui_touch_calibration_screen_dot_top_left, LV_ALIGN_TOP_LEFT, 5, 5);

    ui_touch_calibration_screen_dot_bottom_right = lv_obj_create(ui_touch_calibration_screen);
    lv_obj_set_size(ui_touch_calibration_screen_dot_bottom_right, 10, 10);
    lv_obj_set_style_radius(ui_touch_calibration_screen_dot_bottom_right, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui_touch_calibration_screen_dot_bottom_right, lv_color_make(255, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui_touch_calibration_screen_dot_bottom_right, 0, LV_PART_MAIN);
    lv_obj_align(ui_touch_calibration_screen_dot_bottom_right, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
    lv_obj_add_flag(ui_touch_calibration_screen_dot_bottom_right, LV_OBJ_FLAG_HIDDEN);
}

void ui_touch_calibration_screen_step_2()
{
    if (ui_touch_calibration_screen_dot_top_left != NULL)
    {
        lv_obj_del(ui_touch_calibration_screen_dot_top_left);
        ui_touch_calibration_screen_dot_top_left = NULL;
    }

    lv_label_set_text(ui_touch_calibration_screen_label, "Touch and hold\nthe bottom-right corner\nuntil the next instruction appears.");
    lv_obj_clear_flag(ui_touch_calibration_screen_dot_bottom_right, LV_OBJ_FLAG_HIDDEN);
}

void ui_touch_calibration_screen_step_3()
{
    if (ui_touch_calibration_screen_dot_bottom_right != NULL)
    {
        lv_obj_del(ui_touch_calibration_screen_dot_bottom_right);
        ui_touch_calibration_screen_dot_bottom_right = NULL;
    }

    lv_label_set_text(ui_touch_calibration_screen_label, "Touch calibration complete!");
}

void ui_touch_calibration_screen_destroy()
{
    if (ui_touch_calibration_screen != NULL)
    {
        lv_obj_del(ui_touch_calibration_screen);
        ui_touch_calibration_screen = NULL;
    }
}
