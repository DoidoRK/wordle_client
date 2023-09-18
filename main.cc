#include <iostream>
#include "conio_linux.h"
#include <unistd.h> 

using namespace std;

void printSquare(int startX,int startY, char character, int color) {
    int squareSize = 5;
    setfontcolor(color);
    // Loop to print each row of the square
    for (int row = startY; row < startY+squareSize; ++row) {
        // Position the cursor to the appropriate row and column
        gotoxy(startX * (squareSize + 1) + 1, row + 1);
        
        // Print the first and last rows with '*' characters
        if (row == startY || row == startY+(squareSize - 1)) {
            for (int col = 0; col < squareSize; ++col) {
                cout << "*";
            }
        } else {
            // Print the middle rows with '*' at the edges and spaces inside
            cout << "*";
            for (int col = startX+1; col < startX+(squareSize - 1); ++col) {
                if(row == startY+(squareSize/2) && col == startX+(squareSize/2)){
                    cout << character;
                } else {
                    cout << " ";
                }
            }
            cout << "*";
        }
    }
}

void printRow() {
    // printSquare(0,0,'A');
    // printSquare(1,0,'A');
    // printSquare(2,0,'A');
    // printSquare(3,0,'A');
    // printSquare(4,0,'A');

    printSquare(0,0,'A',RED);
    printSquare(0,5,'A',GREEN);
    printSquare(2,10,'A',YELLOW);
    printSquare(3,15,'A',WHITE);
    printSquare(4,20,'A',RED);
    
}

int main() {
    clrscr();
    printRow();
    cout << endl;
}