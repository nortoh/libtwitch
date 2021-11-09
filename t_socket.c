#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"

static int sock = 0;
static char* buffer[1024];
static struct sockaddr_in serv_addr;

int t_conn(char* host, int port) {
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    info(__FILE__, "Created socket\n");
    
    // Convert IP to binary form
    if(inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        info(__FILE__, "Failed to convert IP to binary form\n");
        return -1;
    }

    // Connect socket to host
    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        info(__FILE__, "Connection failed\n");
        return -1;
    }

    info(__FILE__, "Connected!\n");

    return 1;
}

int raw(char* raw) {
    if(!sock) return -1;
    return send(sock, raw, strlen(raw), 0);
}