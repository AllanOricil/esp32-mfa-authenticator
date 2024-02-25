// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"
#include "totp-map.h"
#include "esp_log.h"
#include <time.h>

extern int setup_complete;
extern uint32_t LV_EVENT_SETUP_COMPLETE;


typedef struct {
    int index;
} TotpValueChangedEventData;


// TOTP SCREEN EVENTS

void notify_totp_labels_to_refresh(lv_timer_t *timer) {
    LV_LOG_USER("timer_callback called.");  //this line is added to log message
    lv_obj_t *screen = (lv_obj_t*)timer->user_data;
    lv_obj_t *child;
    int index = 0;
    // Get the first child of the screen
    child = lv_obj_get_child(screen, NULL);

    while (child) {
        LV_LOG_USER("INSIDE WHILE");
        if (lv_obj_check_type(child, &lv_label_class)) {
             LV_LOG_USER("FOUND A CHILD LABEL");
            TotpValueChangedEventData data;
            data.index = index;
            if(data.index != NULL) {
                LV_LOG_USER("DATA.INDEX HAS VALUE");
                LV_LOG_USER(data.index);
            }
            LV_LOG_USER("REACHED THIS POINT");
            lv_event_send(child, LV_EVENT_VALUE_CHANGED, &data);
            LV_LOG_USER("STANDARD EVENT DISPATCHED");
        }
         LV_LOG_USER("OUTSIDE WHILE");
        child = lv_obj_get_child(screen, child);
        index++;
    }
}


void update_bar(lv_timer_t *timer) {
    time_t now;
    struct tm * timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    
    // Get your bar object from user data
    lv_obj_t * bar = (lv_obj_t*)timer->user_data;
    
    // Adjust the value regarding current seconds
    int val = 30 - timeinfo->tm_sec % 30;

    lv_bar_set_value(bar, val, LV_ANIM_OFF);
}


void on_totp_screen_load_starts(lv_event_t * e){
    LV_LOG_USER("on_totp_screen_load_starts");
    lv_timer_create(notify_totp_labels_to_refresh, 1000, lv_event_get_target(e));


    // Initialize the bar at the start
    time_t now; 
    struct tm * timeinfo; 
    time(&now); 
    timeinfo = localtime(&now); 
    int val = 30 - timeinfo->tm_sec % 30; 
    lv_bar_set_value(ui_Bar2, val, LV_ANIM_OFF); 

    // Create a timer to update the bar every second
    lv_timer_create(update_bar, 1000, ui_Bar2);
}

void on_value_changed(lv_event_t * e) {
    LV_LOG_USER("on_value_changed");
    lv_obj_t * label = lv_event_get_target(e);
    TotpValueChangedEventData * data = (TotpValueChangedEventData *)lv_event_get_param(e);
    char * totp = get_totp_by_index(data->index);
    if (totp){
        lv_label_set_text(label, totp);
    }
}

void on_time_change(lv_event_t * e){
    LV_LOG_USER("on_time_change");
    lv_obj_t * bar = lv_event_get_target(e);
    TotpValueChangedEventData * data = (TotpValueChangedEventData *)lv_event_get_param(e);
    char * totp = get_totp_by_index(data->index);
    if (totp){
        lv_bar_set_value(bar, 30, LV_ANIM_OFF);
    }
}