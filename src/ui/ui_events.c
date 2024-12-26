#include <time.h>
#include <string.h>
#include "ui.h"
#include "services.h"
#include "esp_log.h"
#include "constants.h"
#include "pin.h"
#include "mfa.h"

void on_totp_component_label_value_changed(lv_event_t *e)
{
    LV_LOG_TRACE("on_totp_component_label_value_changed");
    lv_obj_t *label = lv_event_get_target(e);
    TotpValueChangeEvent *data = (TotpValueChangeEvent *)lv_event_get_param(e);
    Service service = get_active_services_group()[data->index];

    LV_LOG_TRACE("totp: %s index: %d", service.totp, data->index);
    lv_label_set_text(label, service.totp);
}

void on_totp_component_countdown_value_changed(lv_event_t *e)
{
    LV_LOG_TRACE("on_totp_component_bar_value_changed");
    lv_obj_t *bar = lv_event_get_target(e);
    struct tm *timeinfo;
    time_t now;
    time(&now);
    timeinfo = localtime(&now);
    int val = TOTP_PERIOD - timeinfo->tm_sec % TOTP_PERIOD;
    lv_bar_set_value(bar, val, LV_ANIM_OFF);
}

void on_totp_screen_gesture(lv_event_t *e)
{
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    printf("DIR: %d\n", dir);

    // NOTE: it is inverted because the gesture event is inverted
    bool group_changed = false;
    if (dir == LV_DIR_LEFT)
    {
        group_changed = change_active_group_right();
    }
    else if (dir == LV_DIR_RIGHT)
    {
        group_changed = change_active_group_left();
    }

    if (group_changed)
    {
        update_totps();
        ui_totp_screen_render_totp_components();
        ui_totp_screen_update_totp_labels();
        ui_totp_screen_update_totp_countdowns();
    }
}

void on_pin_keyboard_button_clicked(lv_event_t *e)
{
    lv_obj_t *keyboard = lv_event_get_target(e);
    uint32_t btn_id = lv_btnmatrix_get_selected_btn(keyboard);
    printf("Debug message: %u\n", btn_id);

    if (btn_id != LV_BTNMATRIX_BTN_NONE)
    {
        const char *btn_text = lv_btnmatrix_get_btn_text(keyboard, btn_id);

        if (strcmp(btn_text, "DEL") == 0)
            lv_textarea_del_char(ui_pin_screen_textarea);
        else if (strcmp(btn_text, "CLEAR") == 0)
            lv_textarea_set_text(ui_pin_screen_textarea, "");
        else if (strcmp(btn_text, "OK") == 0)
            lv_event_send(ui_pin_screen_textarea, LV_EVENT_READY, NULL);
        else
            lv_textarea_add_text(ui_pin_screen_textarea, btn_text);
    }
}

void on_pin_submit(lv_event_t *e)
{
    lv_obj_t *textarea = lv_event_get_target(e);
    const char *pin = lv_textarea_get_text(textarea);

    if (!validate_pin(pin))
    {
        config.number_of_wrong_unlock_attempts--;

        if (!config.number_of_wrong_unlock_attempts)
        {
            lv_obj_center(lv_msgbox_create(NULL, "ERROR", "BOARD LOCKED", NULL, false));
            return;
        }

        char message[128];
        sprintf(message, "Wrong PIN\nAttempts left: %d", config.number_of_wrong_unlock_attempts);
        lv_obj_center(lv_msgbox_create(NULL, "ERROR", message, NULL, true));
        return;
    }

    config.number_of_wrong_unlock_attempts = config.max_number_of_wrong_unlock_attempts;

    lv_scr_load(ui_totp_screen);
}