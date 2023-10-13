#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "config.h"
#include "terminal_gui.h"
#include "utils.h"

int main() {
    int server_socket, N=30;
    struct sockaddr_in server_addr;
    char buff[N];
    clrscr();
    while (true)
    {
        check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to open stream socket");
        //Server Conection
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(IP_SERVER);
        check((connect(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0),"Failed to connect to server");
        recv(server_socket, &buff, sizeof(buff), 0);
        gotoxy(0,2);
        printf("%s", buff);
        sleep(1);
    }
    return 0;
}