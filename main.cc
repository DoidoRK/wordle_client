#include <iostream>
#include "conio_linux.h"
#include <unistd.h> 

using namespace std;

void printRow() {
    // Define the size of each square
    int squareSize = 5;

    // Loop to print five squares side by side
    for (int i = 0; i < 5; ++i) {
        setfontcolor(RED);
        // Loop to print each row of the square
        for (int row = 0; row < squareSize; ++row) {
            // Position the cursor to the appropriate row and column
            gotoxy(i * (squareSize + 1) + 1, row + 1);
            
            // Print the first and last rows with '*' characters
            if (row == 0 || row == squareSize - 1) {
                for (int col = 0; col < squareSize; ++col) {
                    cout << "*";
                }
            } else {
                // Print the middle rows with '*' at the edges and spaces inside
                cout << "*";
                for (int col = 1; col < squareSize - 1; ++col) {
                    cout << " ";
                }
                cout << "*";
            }
        }
    }
}

int main() {
    clrscr();
    printRow();
    cout << endl;
}