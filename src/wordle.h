#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <iostream>
#include "wordle_types.h"

void loginPlayer(user_t *player);
void initializeAttempts(attempt_t attempts[MAX_ATTEMPTS], int max_attempt_n);
const char* sendAttemptToServer(int* current_row, int* current_col, user_t *player, int* current_attempt, string user_input_string, attempt_t attempts[MAX_ATTEMPTS]);
const char* sendTimeOutToServer(user_t player);

#endif /* _WORDLE_H_ */