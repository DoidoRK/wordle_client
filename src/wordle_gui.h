#ifndef _WORDLE_GUI_H_
#define _WORDLE_GUI_H_
#include <iostream>
#include <ncurses.h>
#include "wordle_types.h"

using namespace std;

void printCharacters(int start_y, int start_x, char character, int color) {
    // Determine which color pair to use based on the 'color' variable
    int selectedColorPair = 7;  // Default to white on black
    if (color >= 1 && color <= 3) {
        selectedColorPair = color;
    }

    attron(COLOR_PAIR(selectedColorPair)); // Cycle through the color pairs
    if (character == ' ') {
        mvaddch(start_y, start_x, '-');
    } else {
        mvaddch(start_y, start_x, character);
    }
    // attroff(COLOR_PAIR(selectedColorPair));
}

void printTries(attempt_t attempts[MAX_ATTEMPTS], int word_size) {
    for (int i = 0; i < word_size; i++)
    {
        for (int j = 0; j < MAX_ATTEMPTS; j++)
        {
            printCharacters(j,i,attempts[j].word[i],attempts[i].colors[i]);
        }
    }
}

void printRanking(int start_y, int start_x, user_t player, highscore_t highscore[HIGHSCORE_SIZE], int highscore_size){
    mvprintw(start_y,start_x,"Player: %s\tScore:%d",player.username,player.score);
    mvprintw(start_y+1,start_x,"****Highscores****");
    mvprintw(start_y+2,start_x,"Player\tScore");
    for (size_t i = 0; i < highscore_size; i++)
    {
        mvprintw((start_y+3)+i,start_x,"%s\t%d",highscore[i].username,highscore[i].score);
    }
}

#endif /* _WORDLE_GUI_H_ */
