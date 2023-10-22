#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "communication.h"
#include "utils/string_utils.h"
#include "wordle.h"

using namespace std;

const char* message_strings[] = {
    "INVALID_MESSAGE_TYPE",
    "PLAYER_JOINED",
    "PLAYER_NEW_WORD",
    "PLAYER_ATTEMPT",
    "GET_HIGHSCORE"
};

string printMessage(long unsigned int message) {
    if (message >= 0 && message < sizeof(message_strings) / sizeof(message_strings[0])) {
        return message_strings[message];
    } else {
        return "INVALID_ENUM_VALUE";
    }
}

void loginPlayer(user_t *player) {
    bool valid_username = false;
    string username;
    while (!valid_username)
    {
        cout << "Please enter your name: ";
        cin >> username;
        if(MAX_USERNAME_LEN < username.length()){
            cout << "Invalid username (name is too big)!";
        } else {
            const char *cstr;
            username = toUpperCase(username);
            cstr = username.c_str();
            strncpy(player->username,cstr,MAX_USERNAME_LEN);
            cout << "Nome lido: " << player->username << endl;
            valid_username = true;
        }
    }
    data_packet_t message, response;
    message.message_type = PLAYER_JOINED;
    message.player = *player;
    sendMessageToServer(&message, &response);
    cout << "Mensagem recebida: " << printMessage(response.message_type) << endl;
    cout << "Player recebido: " << response.player.username << endl;
    cout << "Player score: " << response.player.score << endl;
    player->score = response.player.score;
}

void sendAttemptToServer(int* current_row, int* current_col, user_t *player, int* current_attempt, string user_input_string, attempt_t attempts[MAX_ATTEMPTS]) {
    data_packet_t attempt_message, attempt_result;
    player->attempt_n = *current_attempt;
    const char* cstr = user_input_string.c_str();
    strncpy(player->current_attempt.word, cstr, WORD_SIZE);
    attempt_message.player = *player;
    attempt_message.message_type = PLAYER_ATTEMPT;
    sendMessageToServer(&attempt_message, &attempt_result);
    *player = attempt_result.player;
    switch (attempt_result.message_type)
    {
        case INVALID_ATTEMPT_WORD:
            printf("Invalid attempt word!! \n\n\n\n");
            cleanAttempt(attempts,*current_attempt);
            *current_col = 0;
            current_row = 0;
            //Shows invalid attempt message under the user interface.
            break;

        case PLAYER_NEW_WORD:
            printf("Player new word!! \n\n\n\n");
            //Clear attempts and resets current attempts
            initializeAttempts(attempts,MAX_ATTEMPTS);
            *current_col = 0;
            current_row = 0;
            current_attempt = 0;
            //Clear attempts.
            break;

        case PLAYER_ATTEMPT:
            printf("Player attempt!! \n\n\n\n");
            player->score = attempt_result.player.score;
            memcpy(attempts[*current_attempt].colors, attempt_result.player.current_attempt.colors, WORD_SIZE*sizeof(int));
            memcpy(attempts[*current_attempt].word, player->current_attempt.word, WORD_SIZE*sizeof(char));
            *current_col = 0;
            current_row++;
            current_attempt++;
            break;

        default:
            printf("Unknown message from server!! \n\n\n\n");
            printf("Message: %d\nPlayer Name: %s\nPlayer Score: %d\nPlayer attempt word: %s\n",attempt_result.message_type,
                    attempt_result.player.username,
                    attempt_result.player.score,
                    attempt_result.player.current_attempt.word);
            break;
    }
}

void sendTimeOutToServer(user_t player){
    data_packet_t attempt_message, attempt_result;
    attempt_message.player = player;
    attempt_message.message_type = PLAYER_NEW_WORD;
    sendMessageToServer(&attempt_message, &attempt_result);
}