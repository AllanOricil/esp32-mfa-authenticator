#pragma once
#include <string.h>
#include <stdio.h>
#include "constants.h"

// NOTE: define byte if it's not yet defined
typedef unsigned char byte; 

typedef struct {
    int length;
    byte* value;
} DecodedBase32Secret;

extern char keys[MAX_NUMBER_OF_SERVICES][MAX_SERVICE_NAME_LENGTH];
extern DecodedBase32Secret decodedBase32Secrets[MAX_NUMBER_OF_SERVICES];
extern char totps[MAX_NUMBER_OF_SERVICES][MAX_TOTP_LENGTH];

// Current number of elements in the map
extern int size;

#ifdef __cplusplus
extern "C" {
#endif

// to get the index of a key in the keys array
int get_index(char key[]); 



// Function to insert a key-value pair into the map
void upsert_decoded_base32_secret(char key[], DecodedBase32Secret decodedBase32Secret); 

// Function to insert a key-value pair into the map
void upsert_totp(char key[], char value[]); 

// Function to get the value of a key in the map
DecodedBase32Secret* get_decoded_base_32_secret(char key[]); 

// Function to get the totp using the key in the map
char *get_totp(char key[]);

// Function to get the totp using the array index
char* get_totp_by_index(int index);

// Function to print all totps with their respective service name
void print_totps(); 

// Function to print the map
void print(); 

#ifdef __cplusplus
}
#endif
