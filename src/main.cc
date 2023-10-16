#include "config.h"
#include <pthread.h>
#include <chrono>
#include "wordle_types.h"
#include "wordle_gui.h"
#include "unistd.h"
#include "ncurses.h"
#include "string.h"

user_t player;
highscore_t highscore;
attempt_t attempts[MAX_ATTEMPTS];
int current_attempt = 0;

bool quit_program = false;
string right_word = "teste";

pthread_t user_input_thread, timer_thread, gui_thread;

// Mutex for accessing shared data
pthread_mutex_t user_attempt_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

string user_input_string;

int current_row = 0;
int current_col = 0;

void attemptMessage(){
    pthread_mutex_lock(&print_mutex);
    mvprintw(7, 0, "Pressione enter para enviar a palavra");
    pthread_mutex_unlock(&print_mutex);
}

void clearAttemptMessage(){
    pthread_mutex_lock(&print_mutex);
    move(7, 0);
    clrtoeol();
    pthread_mutex_unlock(&print_mutex);
}

void printUserInput(int curent_row, int current_col, char character){
    pthread_mutex_lock(&print_mutex);
    mvprintw(current_row, current_col, "%c", character);
    pthread_mutex_unlock(&print_mutex);
}

// Function to capture user input
void * userInputThread(void * args) {
    const int num_rows = MAX_ATTEMPTS;
    const int num_cols = WORD_SIZE;
    while (!quit_program) {
        int ch = getch();
        if (ch == 27) {  // ESC
            quit_program = true;
        } else if (ch == 10 && current_row == num_rows) {  // Enter key
            pthread_mutex_lock(&user_attempt_mutex);
            player.current_attempt.word = user_input_string;
            pthread_mutex_unlock(&user_attempt_mutex);
            current_col = 0;
            current_row++;
            clearAttemptMessage();
        } else if (ch == KEY_BACKSPACE) {  // Backspace key
            if (!user_input_string.empty() || current_col != 0) {
                user_input_string.pop_back();
                --current_col;
                printUserInput(current_row, current_col, '-');
                clearAttemptMessage();
            }
        } else if(isalpha(ch) && current_col < num_cols) {
            user_input_string.push_back(ch);
            printUserInput(current_row, current_col, ch);
            current_col++;
            clearAttemptMessage();
        }
        if (current_col == num_cols){
            attemptMessage();
        }
        if (current_row == num_rows){
            quit_program = true;
        }
        refresh();
    }
}

// Function to display a timer
void * displayTimer(void *args) {
    int seconds = 0;
    while (!quit_program)
    {
        pthread_mutex_lock(&print_mutex);
        mvprintw(0, 30, "Timer: %02d", (190 - (seconds % 60)));
        pthread_mutex_unlock(&print_mutex);
        refresh();
        sleep(1);
        seconds++;
    }
}

void * displayGUIThread(void *args){
    int tries_printed = 0;
    while (!quit_program) {
        if (tries_printed < 1)
        {
            pthread_mutex_lock(&print_mutex);
            printTries(attempts,WORD_SIZE);
            pthread_mutex_unlock(&print_mutex);
            tries_printed++;
        }
        // displayTimer(&seconds);
        sleep(0.06);
    }
}

int main() {
    initscr();              // Initialize ncurses
    start_color();          // Habilita cores no terminal
    cbreak();               // Line buffering disabled
    noecho();               // Don't echo user input
    keypad(stdscr, TRUE);   // Enable special keys
    curs_set(0);            // Hide cursors

    if (pthread_create(&user_input_thread, NULL, userInputThread, NULL) != 0) {
        endwin();
        fprintf(stderr, "Failed to create the input thread\n");
        return 1;
    }

    if (pthread_create(&gui_thread, NULL, displayGUIThread, NULL) != 0) {
        endwin();
        fprintf(stderr, "Failed to create the GUI thread\n");
        return 1;
    }

    if (pthread_create(&timer_thread, NULL, displayTimer, NULL) != 0) {
        endwin();
        fprintf(stderr, "Failed to create the timer thread\n");
        return 1;
    }

    if (pthread_join(user_input_thread, NULL) != 0) {
        fprintf(stderr, "Failed to join the input thread\n");
        return 1;
    }

    if (pthread_join(gui_thread, NULL) != 0) {
        fprintf(stderr, "Failed to join the GUI thread\n");
        return 1;
    }

    if (pthread_join(timer_thread, NULL) != 0) {
        fprintf(stderr, "Failed to join the timer thread\n");
        return 1;
    }

    endwin();  // Terminate ncurses
    return 0;
}
