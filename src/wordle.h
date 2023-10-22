#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <iostream>
#include "wordle_types.h"

void loginPlayer(user_t *player);
void initializeAttempts(attempt_t attempts[MAX_ATTEMPTS], int max_attempt_n);
void sendAttemptToServer(int* current_row, int* current_col, user_t *player, int* current_attempt, string user_input_string, attempt_t attempts[MAX_ATTEMPTS]);

#endif /* _WORDLE_H_ */