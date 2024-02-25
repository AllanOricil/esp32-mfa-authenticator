#include <stdio.h>
#include <string.h>

#define MAX_SIZE 10 // Due to mem limits there has to exist a max number of services we can generate TOTPs for

typedef unsigned char byte; // define byte if it's not yet defined

typedef struct {
    int length;
    byte* value;
} DecodedBase32Secret;

int size = 0; // Current number of elements in the map 
char keys[MAX_SIZE][10]; // Array to store the keys 
DecodedBase32Secret decodedBase32Secrets[MAX_SIZE]; // Array to store decoded base32 secrets
char totps[MAX_SIZE][7]; // Array to store current totp for each service

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
        printf("%s: %d, %s, %s", keys[i], decodedBase32Secrets[i].length, decodedBase32Secrets[i].value, totps[i]);
    }
}

void print_totps(){
    for(int i = 0; i < size; i++) {
        printf("%s: %d", keys[i], totps[i]);
    }
}
