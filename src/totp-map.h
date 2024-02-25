#pragma once
#include <string.h>
#include <stdio.h>

#define MAX_SIZE 100 // Maximum number of elements in the map

extern char keys[MAX_SIZE][100];
extern char secrets[MAX_SIZE][100]; // Array to store the secrets
extern char totps[MAX_SIZE][100]; // Array to store the totps

// Current number of elements in the map
extern int size;

#ifdef __cplusplus
extern "C" {
#endif

// to get the index of a key in the keys array
int get_index(char key[]); 

// Function to insert a key-value pair into the map
void upsert_secret(char key[], char value[]); 

// Function to insert a key-value pair into the map
void upsert_totp(char key[], char value[]); 

// Function to get the value of a key in the map
char *get_secret(char key[]); 

// Function to get the value of a key in the map
char *get_totp(char key[]); 

// Function to print the map
void print_totps(); 

#ifdef __cplusplus
}
#endif
