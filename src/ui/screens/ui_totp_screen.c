#include "../ui.h"
#include "services.h"
#include "constants.h"

#define LV_LABEL_SCROLL_SPEED (15)

lv_obj_t *create_totp_component(
    lv_obj_t *parent,
    const char *service,
    const char *totp)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_width(container, 150);
    lv_obj_set_height(container, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_bg_color(container, lv_color_make(3, 6, 10), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, 5, 0);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *totp_service = lv_label_create(container);
    lv_label_set_text(totp_service, service);
    lv_obj_set_width(totp_service, LV_PCT(100));
    lv_obj_set_style_text_opa(totp_service, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(totp_service, lv_color_make(254, 254, 254), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(totp_service, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(totp_service, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(totp_service, LV_LABEL_LONG_SCROLL_CIRCULAR);

    lv_obj_t *totp_label = lv_label_create(container);
    lv_label_set_text(totp_label, totp);
    lv_obj_set_style_text_opa(totp_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(totp_label, lv_color_make(254, 254, 254), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(totp_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(totp_label, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *totp_countdown = lv_bar_create(container);
    lv_bar_set_range(totp_countdown, 0, 30);
    lv_bar_set_value(totp_countdown, 30, LV_ANIM_OFF);
    lv_bar_set_start_value(totp_countdown, 0, LV_ANIM_OFF);
    lv_obj_set_width(totp_countdown, LV_PCT(100));
    lv_obj_set_height(totp_countdown, 5);
    lv_obj_set_style_pad_top(totp_countdown, 2, 0);
    lv_obj_set_style_bg_color(totp_countdown, lv_color_make(37, 50, 76), LV_PART_MAIN);
    lv_obj_set_style_bg_color(totp_countdown, lv_color_make(37, 196, 244), LV_PART_INDICATOR);

    lv_obj_add_event_cb(totp_label, ui_event_totp_component_label, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(totp_countdown, ui_event_totp_component_countdown, LV_EVENT_ALL, NULL);

    return container;
}

void ui_totp_screen_init(void)
{
    ui_totp_screen = lv_obj_create(NULL);
    const char *name = TOTP_SCREEN_NAME;
    lv_obj_set_user_data(ui_totp_screen, (void *)name);
    lv_obj_add_flag(ui_totp_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui_totp_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui_totp_screen, lv_color_make(12, 18, 30), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_totp_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_totp_screen, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    // NOTE: set up flex container properties for the screen
    lv_obj_set_flex_flow(ui_totp_screen, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_totp_screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_layout(ui_totp_screen, LV_LAYOUT_FLEX);

    ui_totp_screen_render_totp_components();

    lv_obj_add_event_cb(ui_totp_screen, ui_event_totp_screen, LV_EVENT_ALL, NULL);
}

void ui_totp_screen_render_totp_components(void)
{
    lv_obj_clean(ui_totp_screen);
    for (int i = 0; i < get_active_services_group_length(); i++)
    {
        Service service = get_active_services_group()[i];
        create_totp_component(ui_totp_screen, service.name, service.totp);
    }
}

void ui_totp_screen_update_totp_labels()
{
    int index = 0;
    lv_obj_t *totp_component = lv_obj_get_child(ui_totp_screen, index);
    while (totp_component)
    {
        lv_obj_t *totp_label = lv_obj_get_child(totp_component, 1);
        TotpValueChangeEvent event;
        event.index = index;
        lv_event_send(totp_label, LV_EVENT_VALUE_CHANGED, &event);
        index++;
        totp_component = lv_obj_get_child(ui_totp_screen, index);
    }
}

void ui_totp_screen_update_totp_countdowns()
{
    int index = 0;
    lv_obj_t *totp_component = lv_obj_get_child(ui_totp_screen, index);
    while (totp_component)
    {
        lv_obj_t *totp_countdown = lv_obj_get_child(totp_component, 2);
        TotpValueChangeEvent event;
        event.index = index;
        lv_event_send(totp_countdown, LV_EVENT_VALUE_CHANGED, &event);
        index++;
        totp_component = lv_obj_get_child(ui_totp_screen, index);
    }
}
