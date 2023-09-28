#include <iostream>
#include "conio_linux.h"
#include <unistd.h> 

#define WORD_SIZE 5
#define SQUARE_SIZE 5
#define MAX_TRIES 6
using namespace std;

typedef struct tries_struct{
    char correct_word[WORD_SIZE+1];
    int colors[WORD_SIZE];
} user_tries;

void printSquare(int startX, int startY, char character, int color) {
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

void printRow(char word[WORD_SIZE], int colors[WORD_SIZE]) {
    for (int i = 0; i < WORD_SIZE; i++)
    {
        for (int j = 0; j < MAX_TRIES; j++)
        {
            printSquare(i,j,word[i],colors[i]);
        }
    }
}

// void initializeUserTries(user_tries triesArray[], int numTries) {
//     char empty_word[WORD_SIZE + 1] = "     ";
//     int standard_colors[WORD_SIZE] = {WHITE, WHITE, WHITE, WHITE, WHITE};
//     for (int i = 0; i < numTries; ++i) {
//         strncpy(triesArray[i].correct_word, empty_word, WORD_SIZE + 1);
//         memcpy(triesArray[i].colors, standard_colors, sizeof(standard_colors));
//     }
// }

int main() {
    user_tries tries[MAX_TRIES];
    char correct_word[WORD_SIZE+1] = "teste";
    int colors[WORD_SIZE] = {RED,GREEN,YELLOW,BLUE,MAGENTA};
    clrscr();
    printRow(correct_word,colors);
    cout << endl;
}