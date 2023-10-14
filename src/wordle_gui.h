#ifndef _WORDLE_GUI_H_
#define _WORDLE_GUI_H_
#include <iostream>
#include "libs/conio_linux.h"
#include "wordle_types.h"

//GUI settings
#define SQUARE_SIZE 5

using namespace std;

void printSquares(int startX, int startY, char character, int color) {
    setfontcolor(color);
    // Loop to print each row of the square
    for (int row = startY*(SQUARE_SIZE+1); row < startY*(SQUARE_SIZE+1)+SQUARE_SIZE; ++row) {
        // Position the cursor to the appropriate row and column
        gotoxy(startX * (SQUARE_SIZE) + 1, row+1);
        
        // Print the first and last rows with '*' characters
        if (row == startY* (SQUARE_SIZE + 1) || row == startY*(SQUARE_SIZE + 1)+(SQUARE_SIZE - 1)) {
            for (int col = 0; col < SQUARE_SIZE; ++col) {
                cout << "*";
            }
        } else {
            // Print the middle rows with '*' at the edges and spaces inside
            cout << "*";
            for (int col = startX+1; col < startX+(SQUARE_SIZE - 1); ++col) {
                if(row == startY*(SQUARE_SIZE + 1)+(SQUARE_SIZE/2) && col == startX+(SQUARE_SIZE/2)){
                    cout << character;
                } else {
                    cout << " ";
                }
            }
            cout << "*";
        }
    }
}

void printTries(attempt_t attempts[MAX_ATTEMPTS], int wordSize) {
    for (int i = 0; i < wordSize; i++)
    {
        for (int j = 0; j < MAX_ATTEMPTS; j++)
        {
            printSquares(i,j,attempts[j].word[i],attempts[i].colors[i]);
        }
    }
}

#endif /* _WORDLE_GUI_H_ */