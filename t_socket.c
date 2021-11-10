#define _GNU_SOURCE

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"
#include "config.h"

static int sock = 0, running = 0;
static char buffer[1024] = {0};
static struct sockaddr_in serv_addr;

int raw(char* raw) {
    if(!sock) return -1;
    printf("> Sending %s", raw);
    return send(sock, raw, strlen(raw), 0);
}

int conn(char* host, int port) {

    // Initalize socket
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

void handle(char* raw) {
    // Simple tokenizer
    char* token;
    char* result;
    printf("Building %s\n", raw);
    for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        printf("token: %s\n", token);
    }
    
}

void* thread_start(void *vargs) {

    int connection = conn("44.226.36.141", 6667);

    char* password;
    char* username;
    char* nickname;
    
    asprintf(&password, "PASS %s\r\n", get_config_value("oauth_key"));
    asprintf(&nickname, "NICK %s\r\n", get_config_value("username"));
    asprintf(&username, "USER %s\r\n", get_config_value("username"));
    
    // Caps
    raw(password);
    raw(nickname);
    raw(username);
    
    if(connection < 0) {
        error(__FILE__, "Error calling conn()\n");
    }

    int received = read(sock, buffer, 1024);
    printf("< %s\n", buffer);

    // Simple tokenizer
    char* token;
    char* result;
    for(token = strtok_r(buffer, "\r\n", &result); token != 0; token = strtok_r(0, "\r\n", &result)) {
        handle(token);
    }
}

int connect_to_twitch() {
    // Socket thread identifier
    pthread_t socket_thread;

    running = 1;

    info(__FILE__, "Creating socket thread\n");
    if(pthread_create(&socket_thread, 0, thread_start, 0) != 0) {
        error(__FILE__, "Could not create socket thread!!!\n");
        return 0;
    }

    pthread_join(socket_thread, 0);

    return 1;
}