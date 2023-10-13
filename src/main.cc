#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "config.h"
#include "terminal_gui.h"

int main() {
    int s0, s1, f, length, rval, i, N=30;
    struct sockaddr_in server;
    time_t ticks;
    char buff[N];
    s0=socket(AF_INET, SOCK_STREAM, 0);
    if (s0<0)
    {
        perror("opening stream socket");
        exit(1);
    }
    
    if (s0<0) {
        perror("opening stream socket");
        exit(1);
    }

    //Server Conection
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(IP_SERVER);
    if (connect(s0, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        perror("connectando stream socket");
        exit(0);
    }
    printf("Cliente: Aguardando a hora\n");
    rval = recv(s0, &buff, sizeof(buff), 0);
    printf("%s\n", buff);
    printf("Cliente: terminei!\n");
    return 0;
}