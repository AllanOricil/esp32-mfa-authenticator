#ifndef SERVICES_H
#define SERVICES_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
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
        char totp[MAX_TOTP_LENGTH];
    } Service;

    int find_service_index_in_group_by_name(int group, const char *name);
    bool upsert_service_in_group_by_name(int group, const char *name, int secret_length, uint8_t *secret_value);
    bool upsert_service_totp_in_active_services_group_by_name(const char *name, char totp[]);
    Service *get_service_in_group_by_index(int group, int index);
    Service *get_active_services_group();
    int get_services_group_length(int group);
    int get_active_services_group_length();
    void clear_all_services_groups();
    void clear_all_services_in_group(int group);
    void set_active_group(int group);
    bool change_active_group_left();
    bool change_active_group_right();
    void print_service_group(int group);
    void print_all_services_groups();
    void print_active_services_group();

#ifdef __cplusplus
}

#endif

#endif // SERVICES_H
