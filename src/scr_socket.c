#include "script.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void GScr_socket_init(int entityIndex) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        Scr_AddInt(-1);
        return;
    }
    Scr_AddInt(sockfd);
}

void GScr_socket_bind(int entityIndex) {
    int sockfd = Scr_GetInt(0);
    int port = Scr_GetInt(1);

    struct sockaddr_in local_address;
    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(port);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int result = bind(sockfd, (struct sockaddr *)&local_address, sizeof(local_address));
    Scr_AddInt(result);
}

void GScr_socket_connect(int entityIndex) {
    int sockfd = Scr_GetInt(0);
    char* ip = Scr_GetString(1);
    int port = Scr_GetInt(2);

    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &remote_address.sin_addr);

    int result = connect(sockfd, (struct sockaddr *)&remote_address, sizeof(remote_address));
    Scr_AddInt(result);
}

void GScr_socket_send(int entityIndex) {
    int sockfd = Scr_GetInt(0);
    char* message = Scr_GetString(1);

    int result = send(sockfd, message, strlen(message), 0);
    Scr_AddInt(result); // number of bytes sent
}

void GScr_socket_receive(int entityIndex) {
    int sockfd = Scr_GetInt(0);
    int length = Scr_GetInt(1); // specify the max length to read
    char buffer[length];

    int result = recv(sockfd, buffer, length - 1, 0);
    if(result > 0) {
        buffer[result] = '\0';
        Scr_AddString(buffer);
    } else {
        Scr_AddUndefined();
    }
}

void GScr_socket_close(int entityIndex) {
    int sockfd = Scr_GetInt(0);
    close(sockfd);
}