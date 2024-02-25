#include <stdio.h>
#include <string.h>

#define MAX_SIZE 10 // Maximum number of elements in the map

int size = 0; // Current number of elements in the map 
char keys[MAX_SIZE][10]; // Array to store the keys 
char secrets[MAX_SIZE][65]; // Array to store the secrets
char totps[MAX_SIZE][7]; // Array to store the totps

// Function to get the index of a key in the keys array 
int get_index(char key[]) 
{
    for (int i = 0; i < size; i++) {
        if (strcmp(keys[i], key) == 0) {
            return i;
        }
    }
    return -1; // Key not found
}
  

void upsert_secret(char key[], char secret[])
{
    int index = get_index(key);
    if (index == -1) { // Key not found
        strcpy(keys[size], key);
        strcpy(secrets[size], secret);
        size++;
    } 
    else { // Key found
        strcpy(secrets[index], secret);
    }
}


void upsert_totp(char key[], char totp[])
{
    int index = get_index(key);
    if (index == -1) { // Key not found
        strcpy(keys[size], key);
        strcpy(totps[size], totp);
        size++;
    } 
    else { // Key found
        strcpy(totps[index], totp);
    }
}

// Function to get the secret of a key in the map 
char* get_secret(char key[]) 
{
    int index = get_index(key);
    if (index == -1) { // Key not found
        return NULL;
    } 
    else { // Key found
        return secrets[index];
    } 
}

// Function to get the TOTP of a key in the map 
char* get_totp(char key[]) 
{
    int index = get_index(key);
    if (index == -1) { // Key not found
        return NULL;
    } 
    else { // Key found
        return totps[index];
    } 
}

//Function to print the map
void print_totps() 
{
    for(int i = 0; i < size; i++) {
        printf("%s: %s, %s\n", keys[i], secrets[i], totps[i]);
    }
}
