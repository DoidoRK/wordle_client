#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <iostream>
#include "wordle_types.h"

void loginPlayer(user_t player);
void initializeAttempts(attempt_t attempts[MAX_ATTEMPTS], int max_attempt_n);

#endif /* _WORDLE_H_ */