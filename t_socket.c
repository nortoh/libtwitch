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

#define MAX_RECV_LEN 512 // 512B
#define FULL_BUFFER_MULTIPLE 1000 // 512B * 20 = 51.2KB

static int sock = 0, running = 0, connected = 0, motd = 0, recv_bufferptr = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0, full_buffer_size = 0;
static char recv_buffer[MAX_RECV_LEN] = {0};
static char full_buffer[FULL_BUFFER_MULTIPLE * MAX_RECV_LEN];

int send_raw(char* raw) {
    if(!sock) return -1;
    printf("> Sending %s", raw);
    return send(sock, raw, strlen(raw), 0);
}


char* irc_2_type(char* raw) {
    // printf("RAW LINE: %s\n", raw);
    // printf("%d\n", !(strstr(raw, "PRIVMSG") == 0));
    if(!(strstr(raw, "PRIVMSG") == 0)) return "PRIVMSG";
    return "NA";
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

int received_id(char* line, int id) {
    char* copy = strdup(line);
    char* token;
    char* result;

    int count = 0;
    int received_id = 0;

    for(token = strtok_r(copy, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        if(count != 1) {
            count++;
            continue;
        }
        received_id = atoi(token);
        break;    
    }
    // printf("%s\n", copy);
    free(copy);
    return received_id == id;
}

void handle_privmsg(char* raw) {
    printf("Handling this Private Message Line: %s\n", raw);

    char* token;
    char* result;
    int count = 0;

    for(token = strtok_r(raw, "", &result); token != 0; token = strtok_r(0, " ", &result)) {
        


        count++;    
    }
}


void handle(char* raw) {
    char* raw_copy = strdup(raw);

    // Checks
    if(!connected) { 
        connected = received_id(raw, 1);
        send_raw("JOIN #xqcow\r\n");
    }

    if(!motd) {
        motd = received_id(raw, 376);
        return;
    } 

    char* irc_type = irc_2_type(raw);
    printf("Received: %s\n", irc_type);

    if(!strcmp("PRIVMSG", irc_type)) {
        handle_privmsg(raw);
    }

    // printf("%s\n", irc_type);
    // if(strcmp(irc_type, "PRIVMSG")) {
    //     printf("GOT PRIVMSG FOR %s\n", raw_copy);
    // }

    // return;

    // Simple tokenizer
    // char* token;
    // char* result;
    // char* parts[10];
    // // printf("Building %s\n", raw);
    // for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
    //     printf("token: %s\n", token);
    // }
    
}

void receive_full_chunk(int* more_flag) {
    
    // Clear the recv_buffer before reading in more
    memset(recv_buffer, 0, MAX_RECV_LEN);

    if((bytes_recv = recv(sock, recv_buffer, MAX_RECV_LEN, 0)) > 0) {
        int newline_flag = recv_buffer[bytes_recv - 1] == '\n';
        int carriage_flag = recv_buffer[bytes_recv - 2] == '\r';

        memmove(full_buffer + full_buffer_size, recv_buffer, bytes_recv * sizeof(char));
        full_buffer_size += bytes_recv;

        if(newline_flag && carriage_flag) {
            *more_flag = 0;
        }
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
    
    // Auth
    send_raw(password);
    send_raw(nickname);
    send_raw(username);

    // Caps
    send_raw("CAP REQ :twitch.tv/commands\r\n");
    send_raw("CAP REQ :twitch.tv/tags\r\n");
    send_raw("CAP REQ :twitch.tv/membership\r\n");

    if(connection < 0) {
        error(__FILE__, "Error calling conn()\n");
    }

    int more_flag = 0;

    // Clear recv_buffer
    memset(recv_buffer, 0, MAX_RECV_LEN);

    while(running) {

        /*
        Read in a stream of up to 512B
        */
        if((bytes_recv = recv(sock, recv_buffer, MAX_RECV_LEN, 0)) > 0) {

            // Check if our buffer ends with /r/n
            int newline_flag = recv_buffer[bytes_recv - 1] == '\n';
            int carriage_flag = recv_buffer[bytes_recv - 2] == '\r';

            // If it does not, dump the recv_buffer into the full buffer
            // and continue loading more data until /r/n
            if(!(newline_flag && carriage_flag)) {
                memmove(full_buffer, recv_buffer, bytes_recv);
                full_buffer_size += bytes_recv;
                more_flag = 1;

                while(more_flag) {
                    receive_full_chunk(&more_flag);
                }
            } else {
                // We have /r/n, copy the recv_buffer to the full_buffer
                memmove(full_buffer, recv_buffer, bytes_recv);
            }

            // Add null termination 
            full_buffer[full_buffer_size] = '\0';
            full_buffer_size = 0;

            char* token;
            char* result;
            for(token = strtok_r(full_buffer, "\r\n", &result); token != 0; token = strtok_r(0, "\r\n", &result)) {
                printf("< %s\n", token);
                handle(token);
            }

            // Clear memory when we are done
            memset(full_buffer, 0, FULL_BUFFER_MULTIPLE * MAX_RECV_LEN);
            memset(recv_buffer, 0, MAX_RECV_LEN);
        }
    }

    printf("Closing socket\n");
    if(!close(sock)) {
        printf("Socket closed\n");
    } else {
        printf("Error closing socket");
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