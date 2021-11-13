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
#define OVERFLOW_SIZE 1000 // 512B * 20 = 51.2KB

static int sock = 0, running = 0, connected = 0, motd = 0, bufferptr = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0, overflow_size = 0;
static char buffer[MAX_RECV_LEN] = {0};
static char overflow_buffer[OVERFLOW_SIZE * MAX_RECV_LEN];

int send_raw(char* raw) {
    if(!sock) return -1;
    printf("> Sending %s", raw);
    return send(sock, raw, strlen(raw), 0);
}


char* irc_2_type(char* raw) {
    // printf("RAW LINE: %s\n", raw);
    if(!strstr(raw, "PRIVMSG")) return "PRIVMSG";
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


void handle(char* raw) {
    char* raw_copy = strdup(raw);

    // Checks
    if(!connected) { 
        connected = received_id(raw, 1);
    }

    if(!motd) {
        motd = received_id(raw, 376);
        return;
    } 

    char* irc_type = irc_2_type(raw);
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
    //     // printf("token: %s\n", token);

    // }
    
}

void receive_full_chunk(int* more_flag) {
    
    // Clear the buffer before reading in more
    memset((void *) buffer, 0, MAX_RECV_LEN);

    if((bytes_recv = recv(sock, buffer, MAX_RECV_LEN, 0)) > 0) {
        int newline_flag = buffer[bytes_recv - 1] == '\n';
        int carriage_flag = buffer[bytes_recv - 2] == '\r';

        // printf("[%p] Sticking %ld at %p\n", (void *) overflow_buffer, bytes_recv, (void *) overflow_buffer + overflow_size);
        memmove(overflow_buffer + overflow_size, buffer, bytes_recv * sizeof(char));
        overflow_size += bytes_recv;

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

    // send_raw("JOIN #s1mple\r\n");
    // send_raw("JOIN #auronplay\r\n");
    send_raw("JOIN #xqcow\r\n");

    if(connection < 0) {
        error(__FILE__, "Error calling conn()\n");
    }

    int more_flag = 0;
    int cycles = 0;

    // Clear buffer
    memset(buffer, 0, MAX_RECV_LEN);

    while(running) {

        // Get some data
        if((bytes_recv = recv(sock, buffer, MAX_RECV_LEN, 0)) > 0) {

            int newline_flag = buffer[bytes_recv - 1] == '\n';
            int carriage_flag = buffer[bytes_recv - 2] == '\r';
            // printf("[nl: %d, cr: %d, r: %d] %s", newline_flag, carriage_flag, !(newline_flag && carriage_flag), buffer);
            // printf("\n\n");

            if(!(newline_flag && carriage_flag)) {
                memmove(overflow_buffer, buffer, bytes_recv);
                overflow_size += bytes_recv;
                more_flag = 1;

                
                while(more_flag) {
                    receive_full_chunk(&more_flag);
                    cycles++;
                }
                overflow_buffer[overflow_size] = '\0';

                overflow_size = 0;
                cycles = 0;
            } else {
                // Copy the buffer to the overflow buffer
                memmove(overflow_buffer, buffer, bytes_recv);
            }

            printf("%s\n", overflow_buffer);

            // Clear memory when we are done
            memset(overflow_buffer, 0, OVERFLOW_SIZE * MAX_RECV_LEN);
            memset(buffer, 0, MAX_RECV_LEN);
        }
        /*

        // Continously receive data from socket as long as we are not receive a buffer
        // whose size - 2 & size - 1 characters do not respectively match \r\n.
        do {
            if((size_recv = recv(sock, chunk, MAX_RECV_LEN, 0)) > 0) {
                if(size_recv == -1) {
                    printf("Connection aborted");
                    exit(1);
                }

                int newline_flag = chunk[size_recv - 1] == '\n';
                int carriage_flag = chunk[size_recv - 2] == '\r';

                // We did not detect /r/n at the end of the buffer
                if(!(newline_flag && carriage_flag)) {
                    printf("WE ARE GONNA SET MORE_FLAG = 1");
                    // Copy buffer to overflow and clear chunk buffer
                    memcpy((void *)overflow_buffer + overflow_size, chunk, sizeof(chunk) * sizeof(char) + 1);
                    memset(chunk, 0, MAX_RECV_LEN);
                    
                    printf("@@@ CURRENT OVERFLOW @@@\n");
                    printf("%s\n", overflow_buffer);
                    printf("@@@@@@@@@@@@@@@@@@@@@@@@\n");

                    overflow_size += size_recv;
                    more_flag = 1;
                } else {
                // Detected /r/n at the end of the stream

                    // If we were gathering more data
                    if(more_flag) {
                        // handle(overflow_buffer);
                        overflow_buffer[overflow_size] = '\0';
                        printf("$$$ FINAL OVERFLOW $$$\n");
                        printf("%s\n", overflow_buffer);
                        printf("$$$$$$$$$$$$$$$$$$$$$$\n");
                    } else {
                    // We were not gathering more data prior
                        // memcpy((void *)overflow_buffer, chunk, sizeof(chunk) * sizeof(char));
                        // overflow_buffer[size_recv] = '\0';
                        // printf("^^^ FINAL CHUNK ^^^\n");
                        // printf("%s\n", overflow_buffer);
                        // printf("^^^^^^^^^^^^^^^^^^^\n");
                        printf("Chunk: %s\n", chunk);
                    }

                    overflow_size = 0;
                    more_flag = 0;
                    memset(overflow_buffer, 0, OVERFLOW_SIZE * MAX_RECV_LEN);
                    break;
                }
            }
        } while(more_flag);

        */
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