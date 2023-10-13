#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string.h>
#include <inttypes.h>

#define MAX_TRIES 6
#define USERNAME_LEN 64
#define MAX_WORD_LEN 5
#define WORD_SIZE 5
#define SQUARE_SIZE 5

using namespace std;

typedef struct tries_struct{
    string word;
    uint8_t colors[WORD_SIZE];
} user_tries;

typedef struct user_struct{
    uint8_t username;
    uint8_t attempt;
} user;

#endif /* _CONFIG_H_ */