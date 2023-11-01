#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <iostream>
#include "wordle_types.h"

void loginPlayer(user_t *player);
void initializeAttempts(attempt_t attempts[MAX_ATTEMPTS], int max_attempt_n);
const string sendAttemptToServer(int* current_row, int* current_col, user_t* player, int* current_attempt, attempt_t attempts[MAX_ATTEMPTS], size_t word_size, size_t max_attempts, int* seconds);
const string sendTimeOutToServer(user_t player);
void getPlayerRankingFromServer(highscore_t highscore[HIGHSCORE_SIZE], user_t* player);

#endif /* _WORDLE_H_ */