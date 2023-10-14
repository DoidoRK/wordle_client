#include <string.h>
#include "wordle.h"
#include "wordle_gui.h"
#include "utils/string_utils.h"
#include "communication_h"

using namespace std;

void loginPlayer(user_t player) {
    bool correctUsername = false;
    string username;
    while (!correctUsername)
    {
        clrscr();
        gotoxy(0,2);
        cout << "Please enter your name: ";
        cin >> username;
        if(MAX_USERNAME_LEN < username.length()){
            clrscr();
            gotoxy(0,1);
            cout << "Invalid username (name is too big)!";
        } else {
            char tmp[MAX_USERNAME_LEN];
            username = toUpperCase(username);
            for (int i = 0; i < (int)username.length(); i++)
            {
                tmp[i] = username[i];
            }
            correctUsername = true;
            memcpy(player.username,tmp,MAX_USERNAME_LEN);
            cout << "Nome lido: " << player.username << endl;
        }
    }
    data_packet_t data;
    data.message_type = PLAYER_JOINED;
    // memcpy(data.player,player,sizeof(user_t));
    // sendMessageToServer(&data);
}