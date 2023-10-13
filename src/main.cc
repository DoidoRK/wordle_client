#include "config.h"
#include "terminal_gui.h"

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
    cout << "Please enter your username: ";
    cin >> username;
    user_tries tries[MAX_TRIES];
    initializeUserTries(tries, MAX_TRIES);
    while(current_try < MAX_TRIES)
    {
        clrscr();
        printTries(tries);
        cout << endl;
        cout << "Guess the word: ";
        cin >> tries[current_try].word;
        current_try++;
    }
    printTries(tries);
    gotoxy(0,38);
}