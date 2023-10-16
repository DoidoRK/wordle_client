#ifndef _WORDLE_GUI_H_
#define _WORDLE_GUI_H_
#include <iostream>
#include <ncurses.h>
#include "wordle_types.h"

using namespace std;

void printCharacters(int start_y, int start_x, char character, int color) {
    init_pair(1, color, COLOR_BLACK);  // Define a color pair with red text on a black background
    attron(COLOR_PAIR(1));  // Enable the color pair
    if(character == ' '){
        mvprintw(start_y, start_x, "-");
    } else {
        mvprintw(start_y, start_x, "%c", character);
    }
    attroff(COLOR_PAIR(1));  // Disable the color pair
}

void printTries(attempt_t attempts[MAX_ATTEMPTS], int word_size) {
    for (int i = 0; i < word_size; i++)
    {
        for (int j = 0; j < MAX_ATTEMPTS; j++)
        {
            printCharacters(i,j,attempts[j].word[i],attempts[i].colors[i]);
        }
    }
    refresh();
}

#endif /* _WORDLE_GUI_H_ */
