#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <iostream>
#include <string>
#include <vector>
#include "../wordle_types.h"

using namespace std;

string toUpperCase(const string& input) {
    string result = input;

    for (char& c : result) {
        c = toupper(c);
    }

    return result;
}

void initializeAttempts(attempt_t attempts[MAX_ATTEMPTS], int max_attempt_n) {
    char empty_word[WORD_SIZE] = {' ',' ',' ',' ',' '};
    int standard_colors[WORD_SIZE] = {COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE};
    for (int i = 0; i < max_attempt_n; ++i) {
        strncpy(attempts[i].word, empty_word, WORD_SIZE);
        memcpy(attempts[i].colors, standard_colors, sizeof(standard_colors));
    }
}

void cleanAttempt(attempt_t attempts[MAX_ATTEMPTS], int current_attempt){
    char empty_word[WORD_SIZE] = {' ',' ',' ',' ',' '};
    int standard_colors[WORD_SIZE] = {COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE};
    strncpy(attempts[current_attempt].word, empty_word, WORD_SIZE);
    memcpy(attempts[current_attempt].colors, standard_colors, sizeof(standard_colors));
}

#endif /* _STRING_UTILS_H_ */