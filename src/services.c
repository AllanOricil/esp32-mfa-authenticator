#include "services.h"

Service services_groups[MAX_NUMBER_OF_GROUPS][MAX_NUMBER_OF_SERVICES] = {0};
int services_groups_lengths[MAX_NUMBER_OF_GROUPS] = {0};
int active_group = 0;

int find_service_index_in_group_by_name(int group, const char *name)
{
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        printf("Invalid group number\n");
        return -1;
    }

    for (int i = 0; i < MAX_NUMBER_OF_SERVICES; ++i)
    {
        if (strcmp(services_groups[group][i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

bool upsert_service_in_group_by_name(int group, const char *name, int secret_length, uint8_t *secret_value)
{
    printf("Adding service %s in group %d\n", name, group);
    int index = find_service_index_in_group_by_name(group, name);
    if (index == -1)
    {
        if (services_groups_lengths[group] >= MAX_NUMBER_OF_SERVICES)
        {
            printf("No space available in the group\n");
            return false;
        }

        printf("New service is going to be added");
        strcpy(services_groups[group][services_groups_lengths[group]].name, name);
        services_groups[group][services_groups_lengths[group]].secret.length = secret_length;
        services_groups[group][services_groups_lengths[group]].secret.value = secret_value;
        services_groups_lengths[group]++;
    }
    else
    {
        printf("Updating service found at group %d and index %d", group, index);
        services_groups[group][index].secret.length = secret_length;
        services_groups[group][index].secret.value = secret_value;
    }

    return true;
}

bool upsert_service_totp_in_active_services_group_by_name(const char *name, char totp[])
{
    int index = find_service_index_in_group_by_name(active_group, name);
    if (index == -1)
    {
        if (services_groups_lengths[active_group] >= MAX_NUMBER_OF_SERVICES)
        {
            printf("No space available in the group\n");
            return false;
        }

        strcpy(services_groups[active_group][services_groups_lengths[active_group]].name, name);
        strcpy(services_groups[active_group][services_groups_lengths[active_group]].totp, totp);
        services_groups_lengths[active_group]++;
    }
    else
    {
        strcpy(services_groups[active_group][index].totp, totp);
    }

    return true;
}

Service *get_service_in_group_by_index(int group, int index)
{
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        printf("Invalid group number\n");
        return NULL;
    }

    if (index < 0 || index >= services_groups_lengths[group])
    {
        printf("Invalid service index\n");
        return NULL;
    }

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
    if (group < 0 || group >= MAX_NUMBER_OF_GROUPS)
    {
        printf("Invalid group number\n");
        return;
    }

    memset(services_groups[group], 0, sizeof(services_groups[group]));
    services_groups_lengths[group] = 0;
}

void set_active_group(int group)
{
    active_group = group;
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
        printf("Invalid group number: %d\n", group);
        return;
    }

    printf("Group %d:\n", group);
    for (int i = 0; i < services_groups_lengths[group]; ++i)
    {
        const Service *service = &services_groups[group][i];
        printf("  Name: %s\n", service->name);
        printf("  Secret Length: %d\n", service->secret.length);
        if (service->secret.value)
        {
            printf("  Secret Value: ");
            for (int j = 0; j < service->secret.length; ++j)
            {
                printf("%02X ", service->secret.value[j]);
            }
            printf("\n");
        }
        else
        {
            printf("  Secret Value: NULL\n");
        }
        printf("  TOTP: %s\n", service->totp);
        printf("----------------------\n");
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