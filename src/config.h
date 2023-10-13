#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string.h>
#include <inttypes.h>

//Connection settings.
#define PORT 8080
#define IP_SERVER "127.0.0.1"
#define SOCKETERROR (-1)

//Wordle settings.
#define MAX_ATTEMPTS 6
#define USERNAME_LEN 64
#define WORD_SIZE 5
#define HIGHSCORE_PLAYER_NUM 5

//GUI settings
#define SQUARE_SIZE 5

typedef struct attempt_struct{
    uint8_t word[WORD_SIZE];
    uint8_t colors[WORD_SIZE];
} attempt_t;

typedef struct user_struct{
    uint8_t username[USERNAME_LEN];
    uint8_t attempt;
    uint16_t score;
    attempt_t attempts[MAX_ATTEMPTS];
} user_t;

typedef struct highscore_struct{
    uint8_t username[USERNAME_LEN];
    uint16_t score;
} highscore_t;

#endif /* _CONFIG_H_ */