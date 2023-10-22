#include <pthread.h>
#include <chrono>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "wordle_types.h"
#include "wordle_gui.h"
#include "wordle.h"

user_t player;
highscore_t highscore;
attempt_t attempts[MAX_ATTEMPTS];
int current_attempt = 0;

data_packet_t message, result;

bool quit_program = false;
string right_word = "teste";

pthread_t user_input_thread, timer_thread, gui_thread;

// Mutex for accessing shared data
pthread_mutex_t user_attempt_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t user_input_detected = PTHREAD_COND_INITIALIZER;

using namespace std;

string user_input_string;

int current_row = 0;
int current_col = 0;

void showAttemptMessage(string message){
    pthread_mutex_lock(&print_mutex);
    mvprintw(7, 0, "%s", message);
    // mvprintw(7, 0, "Pressione enter para enviar a palavra");
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
        pthread_cond_signal(&user_input_detected);
        if (ch == 27) {  // ESC
            quit_program = true;
        } else if (ch == 10 && current_col == num_cols) {  // Enter key
            pthread_mutex_lock(&user_attempt_mutex);
            sendAttemptToServer(&current_row, &current_col , &player, &current_attempt,user_input_string, attempts);
            pthread_mutex_unlock(&user_attempt_mutex);
            clearAttemptMessage();
        } else if (ch == KEY_BACKSPACE) {  // Backspace key
            if (!user_input_string.empty() || current_col > 0) {
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
            showAttemptMessage("Pressione enter para enviar a palavra");
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
        mvprintw(0, 15, "Timer: %02d", (190 - (seconds % 60)));
        pthread_mutex_unlock(&print_mutex);
        // If timer == 0
        //Sends new word message for the server.
        //Cleans up user attempts.
        //Tells the user that the time is up in bottom message.
        refresh();
        sleep(1);
        seconds++;
    }
}

void * displayGUIThread(void *args){
    int attempts_printed = 0;
    initializeAttempts(attempts,WORD_SIZE);
    while (!quit_program) {
        pthread_mutex_lock(&print_mutex);
        pthread_cond_wait(&user_input_detected, &print_mutex);
        printTries(attempts,WORD_SIZE);
        pthread_mutex_unlock(&print_mutex);
        attempts_printed++;
    }
}

int main() {
    loginPlayer(&player);   // Logs player in
    // initscr();              // Initialize ncurses
    // start_color();          // Habilita cores no terminal

    // cbreak();               // Line buffering disabled
    // noecho();               // Don't echo user input
    // keypad(stdscr, TRUE);   // Enable special keys
    // curs_set(0);            // Hide cursors

    // if (pthread_create(&user_input_thread, NULL, userInputThread, NULL) != 0) {
    //     endwin();
    //     fprintf(stderr, "Failed to create the input thread\n");
    //     return 1;
    // }

    // if (pthread_create(&gui_thread, NULL, displayGUIThread, NULL) != 0) {
    //     endwin();
    //     fprintf(stderr, "Failed to create the GUI thread\n");
    //     return 1;
    // }

    // if (pthread_create(&timer_thread, NULL, displayTimer, NULL) != 0) {
    //     endwin();
    //     fprintf(stderr, "Failed to create the timer thread\n");
    //     return 1;
    // }

    // if (pthread_join(user_input_thread, NULL) != 0) {
    //     fprintf(stderr, "Failed to join the input thread\n");
    //     return 1;
    // }

    // if (pthread_join(gui_thread, NULL) != 0) {
    //     fprintf(stderr, "Failed to join the GUI thread\n");
    //     return 1;
    // }

    // if (pthread_join(timer_thread, NULL) != 0) {
    //     fprintf(stderr, "Failed to join the timer thread\n");
    //     return 1;
    // }

    endwin();  // Terminate ncurses
    return 0;
}