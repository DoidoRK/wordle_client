#include <iostream>
#include "conio_linux.h"
#include <unistd.h> 

using namespace std;
int squareSize = 5;

void printSquare(int startX, int startY, char character, int color) {
    setfontcolor(color);
    // Loop to print each row of the square
    for (int row = startY*(squareSize+1); row < startY*(squareSize+1)+squareSize; ++row) {
        // Position the cursor to the appropriate row and column
        gotoxy(startX * (squareSize) + 1, row+1);
        
        // Print the first and last rows with '*' characters
        if (row == startY* (squareSize + 1) || row == startY*(squareSize + 1)+(squareSize - 1)) {
            for (int col = 0; col < squareSize; ++col) {
                cout << "*";
            }
        } else {
            // Print the middle rows with '*' at the edges and spaces inside
            cout << "*";
            for (int col = startX+1; col < startX+(squareSize - 1); ++col) {
                if(row == startY*(squareSize + 1)+(squareSize/2) && col == startX+(squareSize/2)){
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
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printSquare(i,j,(65+i+j),i);
        }   
    }
}

int main() {
    clrscr();
    printRow();
    cout << endl;
}