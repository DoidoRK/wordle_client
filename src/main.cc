#include <pthread.h>
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

using namespace std;

string user_input_string;

int current_row = 0;
int current_col = 0;

void showMessageFromServer(const char message[]) {
    pthread_mutex_lock(&print_mutex);
    mvprintw(7, 0, "%s", message);
    refresh();
    pthread_mutex_unlock(&print_mutex);
}

void clearAttemptMessage() {
    pthread_mutex_lock(&print_mutex);
    move(7, 0);
    clrtoeol();
    refresh();
    pthread_mutex_unlock(&print_mutex);
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
            showMessageFromServer(sendAttemptToServer(&current_row, &current_col, &player, &current_attempt, attempts, WORD_SIZE, MAX_ATTEMPTS));
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
            showMessageFromServer("Pressione enter para enviar a palavra");
        }
        pthread_mutex_unlock(&user_attempt_mutex);
    }
    return NULL;
}

// Function to display a timer
void *displayTimer(void *args) {
    int seconds = 0;
    while (!quit_program) {
        pthread_mutex_lock(&print_mutex);
        mvprintw(0, 15, "Timer: %02d", (TIMER_LIMIT - (seconds % 60)));
        refresh();
        pthread_mutex_unlock(&print_mutex);
        if (TIMER_LIMIT <= seconds) {
            pthread_mutex_lock(&user_attempt_mutex);
            clearAttemptMessage();
            showMessageFromServer(sendTimeOutToServer(player));
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

void *displayGUIThread(void *args) {
    pthread_mutex_lock(&user_attempt_mutex);
    initializeAttempts(attempts, MAX_ATTEMPTS);
    pthread_mutex_unlock(&user_attempt_mutex);
    while (!quit_program) {
        pthread_mutex_lock(&print_mutex);
        printTries(attempts, WORD_SIZE);
        refresh();
        sleep(0.16);
        pthread_mutex_unlock(&print_mutex);
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
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

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