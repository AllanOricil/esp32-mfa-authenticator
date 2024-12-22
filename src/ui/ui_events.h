#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int index;
    } TotpValueChangeEvent;

    void on_totp_component_label_value_changed(lv_event_t *e);
    void on_totp_component_bar_value_changed(lv_event_t *e);
    void on_keyboard_button_clicked(lv_event_t *e);
    void on_validate_pin(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // UI_EVENTS_H
