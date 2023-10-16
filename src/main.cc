#include "config.h"
// #include "wordle.h"
#include "wordle_types.h"
#include "wordle_gui.h"
#include "unistd.h"
#include "pthread.h"
#include "ncurses.h"
#include "string.h"

user_t player;
highscore_t highscore;
attempt_t attempts[MAX_ATTEMPTS];

int running = 1;

using namespace std;


int main() {
    int maxRow, maxCol;
    getmaxyx(stdscr, maxRow, maxCol);

    const int numRows = 6;
    const int numCols = 5;
    const int blockWidth = 7;
    const int blockHeight = 3;

    string inputString;
    int currentRow = 0;
    int currentCol = 0;

    initscr();              // Initialize ncurses
    cbreak();               // Line buffering disabled
    noecho();               // Don't echo user input
    keypad(stdscr, TRUE);   // Enable special keys
    curs_set(0);            // Hide cursor

    while (running) {
        int ch = getch();
        if (ch == 27) {  // Ctrl+Q (ASCII 17)
            running = 0;
        } else if (ch == 10) {  // Enter key
            currentCol = 0;
            currentRow++;
            move((numRows+1) * blockHeight + 1, 0);
            clrtoeol();
        } else if (ch == KEY_BACKSPACE) {  // Backspace key
            if (!inputString.empty() || currentCol != 0) {
                inputString.pop_back();
                --currentCol;
                mvprintw(currentRow * blockHeight + 1, currentCol * blockWidth + 2, " ");
                move((numRows+1) * blockHeight + 1, 0);
                clrtoeol();
            }
        } else if(isalpha(ch) && currentCol < numCols) {
            inputString.push_back(ch);
            mvprintw(currentRow * blockHeight + 1, currentCol * blockWidth + 2, "%c", ch);
            move(currentRow * blockHeight + 1, currentCol * blockWidth + 2);
            currentCol++;
            move((numRows+1) * blockHeight + 1, 0);
            clrtoeol();
        }
        if (currentCol == numCols){
            mvprintw((numRows+1) * blockHeight + 1, 0, "Pressione enter para enviar a palavra");
        }
        if (currentRow == numRows){
            running = 0;
        }
        refresh();
    }
    endwin();  // Terminate ncurses

    // Print the input string after exiting ncurses
    printw("You entered: %s\n", inputString.c_str());
    refresh();
    curs_set(1); // Unhide cursor
    echo(); //Echos user input into the terminal
}