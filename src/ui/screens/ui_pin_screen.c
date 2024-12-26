#include "../ui.h"
#include "constants.h"

void ui_pin_screen_init(void)
{
    ui_pin_screen = lv_obj_create(NULL);
    const char *name = PIN_SCREEN_NAME;
    lv_obj_set_user_data(ui_pin_screen, (void *)name);
    lv_obj_add_flag(ui_pin_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui_pin_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui_pin_screen, lv_color_make(12, 18, 30), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pin_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_pin_screen, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(ui_pin_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_pin_screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_layout(ui_pin_screen, LV_LAYOUT_FLEX);

    ui_pin_screen_textarea = lv_textarea_create(ui_pin_screen);
    lv_textarea_set_text(ui_pin_screen_textarea, "");
    lv_textarea_set_max_length(ui_pin_screen_textarea, PIN_MAX_LENGTH);
    lv_textarea_set_one_line(ui_pin_screen_textarea, true);
    lv_textarea_set_password_mode(ui_pin_screen_textarea, true);
    lv_textarea_set_placeholder_text(ui_pin_screen_textarea, "PIN code");
    lv_obj_align(ui_pin_screen_textarea, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_pos(ui_pin_screen_textarea, 0, 0);
    lv_obj_set_width(ui_pin_screen_textarea, LV_HOR_RES - 10);
    lv_obj_set_height(ui_pin_screen_textarea, 45);
    lv_obj_add_event_cb(ui_pin_screen_textarea, ui_event_pin_textarea, LV_EVENT_ALL, NULL);
    lv_obj_set_style_text_font(ui_pin_screen_textarea, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_align(ui_pin_screen_textarea, LV_TEXT_ALIGN_CENTER, 0);

    static const char *btnm_map[] = {
        "1", "2", "3", "4", "5", "\n",
        "6", "7", "8", "9", "0", "\n",
        "DEL", "CLEAR", "OK", ""};
    lv_obj_t *keyboard = lv_btnmatrix_create(ui_pin_screen);
    lv_btnmatrix_set_map(keyboard, btnm_map);
    lv_obj_set_width(keyboard, LV_HOR_RES - 10);
    lv_obj_set_flex_grow(keyboard, 1);
    lv_obj_align(keyboard, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_event_cb(keyboard, ui_event_pin_keyboard_button, LV_EVENT_ALL, NULL);
}
