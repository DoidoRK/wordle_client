#include <iostream>
#include "conio_linux.h"
#include <unistd.h>
#include <string.h>

using namespace std;

#define WORD_SIZE 5
#define SQUARE_SIZE 5
#define MAX_TRIES 6
#define USERNAME_LEN 64

string correct_word = "teste";
typedef struct tries_struct{
    string word;
    int colors[WORD_SIZE];
} user_tries;

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

void printTries(user_tries triesArray[MAX_TRIES]) {
    for (int i = 0; i < WORD_SIZE; i++)
    {
        for (int j = 0; j < MAX_TRIES; j++)
        {
            printSquares(i,j,triesArray[j].word[i],triesArray[i].colors[i]);
        }
    }
}

void initializeUserTries(user_tries triesArray[MAX_TRIES], int numTries) {
    string empty_word = "     ";
    int standard_colors[WORD_SIZE] = {WHITE, WHITE, WHITE, WHITE, WHITE};
    for (int i = 0; i < numTries; ++i) {
        triesArray[i].word = empty_word;
        memcpy(triesArray[i].colors, standard_colors, sizeof(standard_colors));
    }
}

int main() {
    int current_try = 0;
    string username;
    string guessed_word;
    std::cout << "Please enter your username: ";
    std::cin >> username;
    user_tries tries[MAX_TRIES];
    initializeUserTries(tries, MAX_TRIES);
    while(current_try < MAX_TRIES)
    {
        clrscr();
        printTries(tries);
        cout << endl;
        std::cout << "Guess the word: ";
        std::cin >> tries[current_try].word;
        current_try++;
    }
    printTries(tries);
}