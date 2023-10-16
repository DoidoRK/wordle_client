#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "wordle_types.h"
#include "utils/network_utils.h"

void sendMessageToServer(data_packet_t *data, data_packet_t *result){
    int server_socket;
    struct sockaddr_in server_addr;

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to open stream socket");

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP_SERVER);
    check((connect(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0),"Failed to connect to server");
    //Send data packet to server
    send(server_socket, data, sizeof(data_packet_t), 0);
    //Receive message from server
    recv(server_socket, result, sizeof(data_packet_t), 0);

    close(server_socket);
}

#endif /* _COMMUNICATION_H_ */