#include <stdio.h>
#include <string.h>
#include "constants.h"

// NOTE: define byte if it's not yet defined
typedef unsigned char byte; 

typedef struct {
    int length;
    byte* value;
} DecodedBase32Secret;

// NOTE: initialize all stores with null
int size = 0;
char keys[MAX_NUMBER_OF_SERVICES][MAX_SERVICE_NAME_LENGTH] = {{'\0'}};
DecodedBase32Secret decodedBase32Secrets[MAX_NUMBER_OF_SERVICES] = {0};
char totps[MAX_NUMBER_OF_SERVICES][MAX_TOTP_LENGTH] = {{'\0'}};

int get_index(char key[]){
    for (int i = 0; i < size; i++) {
        if (strcmp(keys[i], key) == 0) {
            return i;
        }
    }
    return -1;
}

void upsert_decoded_base32_secret(char key[], DecodedBase32Secret decodedBase32Secret){
    int index = get_index(key);
    if (index == -1) {
        strcpy(keys[size], key);
        decodedBase32Secrets[size] = decodedBase32Secret;
        size++;
    } else {
        decodedBase32Secrets[index] = decodedBase32Secret;
    }
}

void upsert_totp(char key[], char totp[]){
    int index = get_index(key);
    if (index == -1) {
        strcpy(keys[size], key);
        strcpy(totps[size], totp);
        size++;
    } else {
        strcpy(totps[index], totp);
    }
}

DecodedBase32Secret get_decoded_base_32_secret(char key[]) {
    int index = get_index(key);
    if (index == -1) {
        DecodedBase32Secret emptyDecodedBase32Secret;
        emptyDecodedBase32Secret.length = -1;
        emptyDecodedBase32Secret.value = NULL;
        return emptyDecodedBase32Secret;
    } else {
        return decodedBase32Secrets[index];
    } 
}

char* get_totp(char key[]) {
    int index = get_index(key);
    if (index == -1) {
        return NULL;
    } else {
        return totps[index];
    } 
}

char* get_totp_by_index(int index) { 
    if (index >= 0 && index < size) {
        return totps[index]; 
    } 
    return NULL;
}

void print(){
    for(int i = 0; i < size; i++) {
        printf("%s: %d, %s, %s\n", keys[i], decodedBase32Secrets[i].length, decodedBase32Secrets[i].value, totps[i]);
    }
}