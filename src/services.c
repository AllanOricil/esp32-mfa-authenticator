#include "services.h"

static const char *TAG = "services";

Service services[MAX_NUMBER_OF_SERVICES] = {0};
uint8_t services_counter = 0;
uint8_t active_group = 0;
uint8_t groups[MAX_NUMBER_OF_GROUPS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
uint8_t groups_counter = 0;

int find_service_index_by_name_and_group(const char *name, uint8_t group)
{
    ESP_LOGI(TAG, "locating service %s in group %d", name, group);

    for (int i = 0; i < MAX_NUMBER_OF_SERVICES; ++i)
    {
        if (strcmp(services[i].name, name) == 0 && services[i].group == group)
        {
            return i;
        }
    }

    ESP_LOGI(TAG, "could not locate service %s in group %d", name, group);
    return -1;
}

bool does_group_exist(uint8_t group)
{
    ESP_LOGI(TAG, "Checking if group %d exists", group);

    for (int i = 0; i < MAX_NUMBER_OF_GROUPS; i++)
    {
        if (groups[i] == group)
        {
            ESP_LOGI(TAG, "Group %d exists", group);
            return true;
        }
    }

    ESP_LOGI(TAG, "Group %d does not exist", group);
    return false;
}

bool add_group_to_list(uint8_t group)
{
    if (groups_counter >= MAX_NUMBER_OF_GROUPS)
    {
        ESP_LOGE(TAG, "Max number of groups reached. Group %d can't be added.", group);
        return false;
    }

    int i;
    for (i = groups_counter - 1; (i >= 0 && groups[i] > group); i--)
    {
        groups[i + 1] = groups[i];
    }

    groups[i + 1] = group;
    groups_counter++;

    ESP_LOGI(TAG, "Group %d added successfully. Current count: %d", group, groups_counter);
    return true;
}

bool upsert_service_by_name_and_group(const char *name, uint8_t group, int secret_length, uint8_t *secret_value)
{
    ESP_LOGI(TAG, "adding service %s in group %d", name, group);

    if (!does_group_exist(group))
    {
        ESP_LOGD(TAG, "adding new group %d", group);
        if (!add_group_to_list(group))
        {
            ESP_LOGD(TAG, "group not added %d", group);
            return false;
        }
        ESP_LOGD(TAG, "group added %d", group);
    }

    int index = find_service_index_by_name_and_group(name, group);
    if (index == -1)
    {
        if (services_counter >= MAX_NUMBER_OF_SERVICES)
        {
            ESP_LOGE(TAG, "no space available in the store");
            return false;
        }

        ESP_LOGD(TAG, "adding new service");
        strcpy(services[services_counter].name, name);
        services[services_counter].group = group;
        services[services_counter].secret.length = secret_length;
        services[services_counter].secret.value = secret_value;
        services_counter++;
    }
    else
    {
        ESP_LOGD(TAG, "updating service found at group %d and index %d", group, index);
        services[index].secret.length = secret_length;
        services[index].secret.value = secret_value;
        services[index].group = group;
    }

    ESP_LOGI(TAG, "new service added successfully");
    return true;
}

bool update_service_totp_in_active_services_group_by_name(const char *name, char totp[])
{
    ESP_LOGI(TAG, "updating totp %s for service %s and group %d", name, totp, active_group);
    int index = find_service_index_by_name_and_group(name, active_group);
    strcpy(services[index].totp, totp);
    ESP_LOGI(TAG, "totp updated successfully");
    return true;
}

void clear_all_services()
{
    memset(services, 0, sizeof(services));
}

uint8_t get_active_group()
{
    return active_group;
}

Service *get_services()
{
    return services;
}

bool change_active_group_left()
{
    int current_index = -1;
    for (int i = 0; i < groups_counter; ++i)
    {
        if (groups[i] == active_group)
        {
            current_index = i;
            break;
        }
    }

    if (current_index == -1)
    {
        return false;
    }

    if (current_index > 0)
    {
        active_group = groups[current_index - 1];
        return true;
    }

    return false;
}

bool change_active_group_right()
{
    int current_index = -1;
    for (int i = 0; i < groups_counter; ++i)
    {
        if (groups[i] == active_group)
        {
            current_index = i;
            break;
        }
    }

    if (current_index == -1)
    {
        return false;
    }

    if (current_index < groups_counter - 1)
    {
        active_group = groups[current_index + 1];
        return true;
    }

    return false;
}

void print_service(int index)
{
    const Service *service = &services[index];
    ESP_LOGD(TAG, "name: %s", service->name);
    ESP_LOGD(TAG, "secret legnth: %d", service->secret.length);
    ESP_LOGD(TAG, "secret value:");
    for (int j = 0; j < service->secret.length; ++j)
    {
        ESP_LOGD(TAG, "%02X ", service->secret.value[j]);
    }
    ESP_LOGD(TAG, "totp: %s", service->totp);
    ESP_LOGD(TAG, "-------------------");
}

void print_all_services()
{
    for (int i = 0; i < MAX_NUMBER_OF_SERVICES; ++i)
    {
        print_service(i);
    }
}