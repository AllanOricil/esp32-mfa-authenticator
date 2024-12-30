#include "services.h"

static const char *TAG = "services";

Service services_groups[MAX_NUMBER_OF_GROUPS][MAX_NUMBER_OF_SERVICES] = {0};
int services_groups_lengths[MAX_NUMBER_OF_GROUPS] = {0};
int active_group = 0;

int find_service_index_in_group_by_name(int group, const char *name)
{
    ESP_LOGI(TAG, "locating service %s in group %d", name, group);
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        ESP_LOGE(TAG, "invalid group number %d", group);
        return -1;
    }

    for (int i = 0; i < MAX_NUMBER_OF_SERVICES; ++i)
    {
        if (strcmp(services_groups[group][i].name, name) == 0)
        {
            return i;
        }
    }

    ESP_LOGI(TAG, "could not locate service %s in group %d", name, group);
    return -1;
}

bool upsert_service_in_group_by_name(int group, const char *name, int secret_length, uint8_t *secret_value)
{
    ESP_LOGI(TAG, "adding service %s in group %d", name, group);
    int index = find_service_index_in_group_by_name(group, name);
    if (index == -1)
    {
        if (services_groups_lengths[group] >= MAX_NUMBER_OF_SERVICES)
        {
            ESP_LOGE(TAG, "no space available in the group");
            return false;
        }

        ESP_LOGD(TAG, "adding new service");
        strcpy(services_groups[group][services_groups_lengths[group]].name, name);
        services_groups[group][services_groups_lengths[group]].secret.length = secret_length;
        services_groups[group][services_groups_lengths[group]].secret.value = secret_value;
        services_groups_lengths[group]++;
    }
    else
    {
        ESP_LOGD(TAG, "updating service found at group %d and index %d", group, index);
        services_groups[group][index].secret.length = secret_length;
        services_groups[group][index].secret.value = secret_value;
    }

    ESP_LOGI(TAG, "new service added successfully");
    return true;
}

bool upsert_service_totp_in_active_services_group_by_name(const char *name, char totp[])
{
    ESP_LOGI(TAG, "adding totp %s for service %s", name, totp);
    int index = find_service_index_in_group_by_name(active_group, name);
    if (index == -1)
    {
        if (services_groups_lengths[active_group] >= MAX_NUMBER_OF_SERVICES)
        {
            ESP_LOGE(TAG, "no space available in the group");
            return false;
        }

        ESP_LOGD(TAG, "adding new totp");
        strcpy(services_groups[active_group][services_groups_lengths[active_group]].name, name);
        strcpy(services_groups[active_group][services_groups_lengths[active_group]].totp, totp);
        services_groups_lengths[active_group]++;
    }
    else
    {
        ESP_LOGD(TAG, "updating totp");
        strcpy(services_groups[active_group][index].totp, totp);
    }

    ESP_LOGI(TAG, "totp added successfully");
    return true;
}

Service *get_service_in_group_by_index(int group, int index)
{
    ESP_LOGI(TAG, "finding service in group %d with index %d", group, index);
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        ESP_LOGE(TAG, "invalid group nuumber %d", group);
        return NULL;
    }

    if (index < 0 || index >= services_groups_lengths[group])
    {
        ESP_LOGE(TAG, "invalid service index %d", index);
        return NULL;
    }

    ESP_LOGI(TAG, "service found in group %d with index %d", group, index);
    return &services_groups[group][index];
}

Service *get_active_services_group()
{
    return services_groups[active_group];
}

int get_services_group_length(int group)
{
    return services_groups_lengths[group];
}

int get_active_services_group_length()
{
    return get_services_group_length(active_group);
}

void clear_all_services_groups()
{
    memset(services_groups, 0, sizeof(services_groups));
    memset(services_groups_lengths, 0, sizeof(services_groups_lengths));
}

void clear_all_services_in_group(int group)
{
    ESP_LOGI(TAG, "clearing all services in group %d", group);
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        ESP_LOGE(TAG, "invalid group %d", group);
        return;
    }

    memset(services_groups[group], 0, sizeof(services_groups[group]));
    services_groups_lengths[group] = 0;
    ESP_LOGI(TAG, "all services in group %d were cleared", group);
}

void set_active_group(int group)
{
    active_group = group;
}

int get_active_group()
{
    return active_group;
}

bool change_active_group_left()
{
    if (active_group > 0)
    {
        active_group--;
        return true;
    }
    return false;
}

bool change_active_group_right()
{
    if (active_group < MAX_NUMBER_OF_GROUPS - 1)
    {
        active_group++;
        return true;
    }
    return false;
}

void print_service_group(int group)
{
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        ESP_LOGE(TAG, "invalid group %d", group);
        return;
    }

    ESP_LOGD(TAG, "group: %d", group);
    for (int i = 0; i < services_groups_lengths[group]; ++i)
    {
        const Service *service = &services_groups[group][i];
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
}

void print_all_services_groups()
{
    for (int group = 0; group < MAX_NUMBER_OF_GROUPS; ++group)
    {
        print_service_group(group);
    }
}

void print_active_services_group()
{
    print_service_group(active_group);
}