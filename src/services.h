#ifndef SERVICES_H
#define SERVICES_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <esp_log.h>
#include "constants.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // NOTE: define uint8_t if it's not yet defined
    typedef unsigned char uint8_t;
    typedef struct
    {
        char name[MAX_SERVICE_NAME_LENGTH];
        struct
        {
            int length;
            uint8_t *value;
        } secret;
        uint8_t group;
        char totp[MAX_TOTP_LENGTH];
    } Service;

    int find_service_index_by_name_and_group(const char *name, uint8_t group);
    bool upsert_service_by_name_and_group(const char *name, uint8_t group, int secret_length, uint8_t *secret_value);
    bool update_service_totp_in_active_services_group_by_name(const char *name, char totp[]);
    void clear_all_services();
    uint8_t get_active_group();
    Service *get_services();
    bool change_active_group_left();
    bool change_active_group_right();
    void print_all_services();

#ifdef __cplusplus
}

#endif

#endif // SERVICES_H
