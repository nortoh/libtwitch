#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "utils.h"

static int sock = 0, running = 0;
static char* buffer[1024];
static struct sockaddr_in serv_addr;

void* thread_start(void *vargs) {
    int count = 0;
    while(running) {
        if(count < 1000) {
            printf("[%d]\n", count);
            count++;
            continue;
        }
        break;
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

int raw(char* raw) {
    if(!sock) return -1;
    return send(sock, raw, strlen(raw), 0);
}