#include <pthread.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "wordle_types.h"
#include "wordle_gui.h"
#include "wordle.h"

user_t player;
highscore_t highscore[HIGHSCORE_SIZE];
attempt_t attempts[MAX_ATTEMPTS];
int current_attempt = 0;
int seconds = 0;
string gui_message = "";

data_packet_t message, result;

bool quit_program = false;

pthread_t user_input_thread, timer_thread, gui_thread, ranking_thread;

// Mutex for accessing shared data
pthread_mutex_t user_attempt_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gui_printing_mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

string user_input_string;

int current_row = 0;
int current_col = 0;

void clearAttemptMessage() {
    gui_message = "                                                                            ";
}

void showMessageFromServer(string message) {
    clearAttemptMessage();
    gui_message = message;
}


// Function to capture user input
void *userInputThread(void *args) {
    const int num_cols = WORD_SIZE; //There will be a ending string character in the string.
    while (!quit_program) {
        int ch = getch();
        pthread_mutex_lock(&user_attempt_mutex);
        if (ch == 27) {  // ESC
            quit_program = true;
        } else if (ch == 10 && current_col == num_cols) {  // Enter key
            clearAttemptMessage();
            pthread_mutex_lock(&gui_printing_mutex);
            showMessageFromServer(sendAttemptToServer(&current_row, &current_col, &player, &current_attempt, attempts, WORD_SIZE, MAX_ATTEMPTS, &seconds));
            pthread_mutex_unlock(&gui_printing_mutex);
        } else if (ch == KEY_BACKSPACE) {  // Backspace key
            if (!user_input_string.empty() || current_col > 0) {
                user_input_string.pop_back();
                attempts[current_attempt].word[current_col - 1] = '-';
                --current_col;
                clearAttemptMessage();
            }
        } else if (isalpha(ch) && current_col < num_cols) {
            user_input_string.push_back(ch);
            attempts[current_attempt].word[current_col] = ch;
            current_col++;
            clearAttemptMessage();
        }
        if (current_col == num_cols) {
            pthread_mutex_lock(&gui_printing_mutex);
            showMessageFromServer("Pressione enter para enviar a palavra");
            pthread_mutex_unlock(&gui_printing_mutex);
        }
        pthread_mutex_unlock(&user_attempt_mutex);
    }
    return NULL;
}

// Function to display a timer
void *displayTimer(void *args) {
    while (!quit_program) {
        if (TIMER_LIMIT <= seconds) {
            pthread_mutex_lock(&user_attempt_mutex);
            pthread_mutex_lock(&gui_printing_mutex);
            showMessageFromServer(sendTimeOutToServer(player));
            pthread_mutex_unlock(&gui_printing_mutex);
            initializeAttempts(attempts, MAX_ATTEMPTS);
            user_input_string.clear();
            current_row = 0;
            current_col = 0;
            seconds = 0;
            pthread_mutex_unlock(&user_attempt_mutex);
        }
        sleep(1);
        seconds++;
    }
    return NULL;
}

void *displayRankingThread(void *args){
    while (!quit_program) {
        pthread_mutex_lock(&user_attempt_mutex);
        getPlayerRankingFromServer(highscore, &player);
        pthread_mutex_unlock(&user_attempt_mutex);
        sleep(1);
    }
    return NULL;
}

void *displayGUIThread(void *args) {
    pthread_mutex_lock(&user_attempt_mutex);
    initializeAttempts(attempts, MAX_ATTEMPTS);
    pthread_mutex_unlock(&user_attempt_mutex);
    while (!quit_program) {
        pthread_mutex_lock(&user_attempt_mutex);
        printTries(attempts, WORD_SIZE);    //Prints user tries.
        printRanking(1, 15, player, highscore, HIGHSCORE_SIZE); //Print Highscore table
        mvprintw(0, 15, "Timer: %02d", (TIMER_LIMIT - (seconds % 60))); //Prints timer.
        mvprintw(15, 0, "%s", gui_message.c_str()); //Prints message.
        refresh();
        pthread_mutex_unlock(&user_attempt_mutex);
        sleep(0.16);
    }
    return NULL;
}

int main() {
    loginPlayer(&player);   // Gets player data.

    initscr(); // Initialize NCurses
    start_color(); // Enable color support
    noecho(); // Don't echo user input
    keypad(stdscr, TRUE); // Enable special keys
    curs_set(0); // Hide the cursor

    // Initialize colors
    init_pair(COLOR_RED, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_GREEN, COLOR_WHITE, COLOR_GREEN);
    init_pair(COLOR_YELLOW, COLOR_WHITE, COLOR_YELLOW);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);

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

    if (pthread_create(&ranking_thread, NULL, displayRankingThread, NULL) != 0) {
        endwin();
        fprintf(stderr, "Failed to create the ranking thread\n");
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

    if (pthread_join(ranking_thread, NULL) != 0) {
        fprintf(stderr, "Failed to join the timer thread\n");
        return 1;
    }

    endwin();  // Terminate ncurses
    return 0;
}