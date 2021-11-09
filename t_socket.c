#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

static int sock = 0;
static char* buffer[1024];
static struct sockaddr_in serv_addr;

int twitch_connect(char* host, int port) {
    printf("[t_socket.c] ");
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IP to binary form
    if(inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        printf("Connection failed\n");
        return -1;
    }

    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    char* message = "Hello World";
    int request = send(sock, message, strlen(message), 0);
    printf("Got %d\n", request);
}

int raw(char* raw) {
    if(!sock) return -1;
    return send(sock, raw, strlen(raw), 0);
}