#include <ncurses.h>
#include <iostream>
#include <string>
#include "communication.h"
#include "utils/string_utils.h"
#include "wordle.h"

using namespace std;

const char* message_strings[] = {
    "INVALID_MESSAGE_TYPE",
    "INVALID_ATTEMPT_WORD",
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

void loginPlayer(user_t* player) {
    bool valid_username = false;
    string username;
    while (!valid_username) {
        cout << "Please enter your name: ";
        cin >> username;
        if (MAX_USERNAME_LEN < username.length()) {
            cout << "Invalid username (name is too big)!";
        } else {
            username = toUpperCase(username);
            strncpy(player->username, username.c_str(), MAX_USERNAME_LEN);
            cout << "Nome lido: " << player->username << endl;
            valid_username = true;
        }
    }
    data_packet_t message, response;
    message.message_type = PLAYER_NEW_WORD;
    message.player = *player;
    sendMessageToServer(&message, &response);
    cout << "Mensagem recebida: " << printMessage(response.message_type) << endl;
    cout << "Player recebido: " << response.player.username << endl;
    cout << "Player score: " << response.player.score << endl;
    player->score = response.player.score;
}

const char* sendAttemptToServer(int* current_row, int* current_col, user_t* player, int* current_attempt, attempt_t attempts[MAX_ATTEMPTS]) {
    data_packet_t attempt_message, attempt_result;
    player->attempt_n = *current_attempt;
    strncpy(player->current_attempt.word, attempts[*current_attempt].word, WORD_SIZE);
    attempt_message.player = *player;
    attempt_message.message_type = PLAYER_ATTEMPT;
    sendMessageToServer(&attempt_message, &attempt_result);
    switch (attempt_result.message_type) {
        case INVALID_ATTEMPT_WORD:
            cleanAttempt(attempts, *current_attempt);
            *current_col = 0;
            *current_row = 0;
            return "Palavra inválida! A palavra não está no banco de dados.";

        case PLAYER_NEW_WORD:
            *current_col = 0;
            *current_row = 0;
            *current_attempt = 0;
            initializeAttempts(attempts, MAX_ATTEMPTS);
            return "Uma nova palavra foi sorteada para o jogador.";

        case PLAYER_ATTEMPT:
            player->score = attempt_result.player.score;
            memcpy(attempts[*current_attempt].colors, attempt_result.player.current_attempt.colors, WORD_SIZE * sizeof(int));
            strncpy(attempts[*current_attempt].word, player->current_attempt.word, WORD_SIZE);
            *current_col = 0;
            (*current_row)++;
            (*current_attempt)++;
            return "Tentativa verificada";

        default:
            printf("Message: %d\nPlayer Name: %s\nPlayer Score: %d\nPlayer attempt word: %s\n", attempt_result.message_type,
                   attempt_result.player.username,
                   attempt_result.player.score,
                   attempt_result.player.current_attempt.word);
            return "Ocorreu um erro!";
    }
}

const char* sendTimeOutToServer(user_t player) {
    data_packet_t attempt_message, attempt_result;
    attempt_message.player = player;
    attempt_message.message_type = PLAYER_NEW_WORD;
    sendMessageToServer(&attempt_message, &attempt_result);
    return "O tempo acabou, uma nova palavra foi sorteada!";
}