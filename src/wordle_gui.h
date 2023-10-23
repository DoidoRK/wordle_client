#ifndef _WORDLE_GUI_H_
#define _WORDLE_GUI_H_
#include <iostream>
#include <ncurses.h>
#include "wordle_types.h"

using namespace std;

void printCharacters(int start_y, int start_x, char character, int color) {
    // Define color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

    // Determine which color pair to use based on the 'color' variable
    int selectedColorPair = 4;  // Default to white on black
    if (color >= 1 && color <= 3) {
        selectedColorPair = color;
    }

    attron(COLOR_PAIR(selectedColorPair));  // Enable the selected color pair
    if (character == ' ') {
        mvprintw(start_y, start_x, "-");
    } else {
        mvprintw(start_y, start_x, "%c", character);
    }
    attroff(COLOR_PAIR(selectedColorPair));  // Disable the color pair
}

void printTries(attempt_t attempts[MAX_ATTEMPTS], int word_size) {
    for (int i = 0; i < word_size; i++)
    {
        for (int j = 0; j < MAX_ATTEMPTS; j++)
        {
            printCharacters(j,i,attempts[j].word[i],attempts[i].colors[i]);
        }
    }
    refresh();
}

#endif /* _WORDLE_GUI_H_ */
