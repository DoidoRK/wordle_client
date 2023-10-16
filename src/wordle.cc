#include <string.h>
#include "communication.h"
#include "utils/string_utils.h"
#include "unistd.h"
#include "wordle.h"

using namespace std;

const char* message_strings[] = {
    "INVALID_MESSAGE_TYPE",
    "PLAYER_JOINED",
    "PLAYER_NEW_WORD",
    "PLAYER_ATTEMPT",
    "GET_HIGHSCORE"
};

string  printMessage(long unsigned int message) {
    if (message >= 0 && message < sizeof(message_strings) / sizeof(message_strings[0])) {
        return message_strings[message];
    } else {
        return "INVALID_ENUM_VALUE";
    }
}

void loginPlayer(user_t player) {
    bool valid_username = false;
    string username;
    while (!valid_username)
    {
        // clrscr();
        // gotoxy(0,2);
        cout << "Please enter your name: ";
        cin >> username;
        if(MAX_USERNAME_LEN < username.length()){
            // clrscr();
            // gotoxy(0,1);
            cout << "Invalid username (name is too big)!";
        } else {
            char tmp[MAX_USERNAME_LEN];
            username = toUpperCase(username);
            for (int i = 0; i < (int)username.length(); i++)
            {
                tmp[i] = username[i];
            }
            player.username = tmp;
            cout << "Nome lido: " << player.username << endl;
            valid_username = true;
        }
    }
    data_packet_t message, response;
    message.message_type = PLAYER_ATTEMPT;
    message.player = player;
    sendMessageToServer(&message, &response);
    cout << "Mensagem recebida: " << printMessage(response.message_type) << endl;
    cout << "Player recebido: " << response.player.username << endl;
    cout << "Player score: " << response.player.score << endl;
    player.score = response.player.score;
}


// void initializeUserTries(user_tries triesArray[MAX_TRIES], int numTries) {
//     string empty_word = "     ";
//     int standard_colors[WORD_SIZE] = {WHITE, WHITE, WHITE, WHITE, WHITE};
//     for (int i = 0; i < numTries; ++i) {
//         triesArray[i].word = empty_word;
//         memcpy(triesArray[i].colors, standard_colors, sizeof(standard_colors));
//     }
// }