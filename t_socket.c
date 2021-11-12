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

#define MAX_RECV_LEN 512

static int sock = 0, running = 0, connected = 0, motd = 0, bufferptr = 0;
static char buffer[MAX_RECV_LEN]; 
static struct sockaddr_in serv_addr;

int send_raw(char* raw) {
    if(!sock) return -1;
    printf("> Sending %s", raw);
    return send(sock, raw, strlen(raw), 0);
}

int recv_raw(int fd, char* buffer, size_t size) {
    int r = recv(fd, buffer, size, 0);
    if(r <= 0) return -1;
    return r;
}

int new_recv(int fd) {
    int size_recv, total_size = 0;
    char chunk[MAX_RECV_LEN];

    while(1) {
        memset(chunk, 0, MAX_RECV_LEN);
        if((size_recv = recv(fd, chunk, MAX_RECV_LEN, 0)) < 0) {
            break;
        } else {
            total_size += size_recv;
            printf("%s\n", chunk);
        }
    }

    return total_size;
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
        send_raw("JOIN #ibai\r\n");
        send_raw("JOIN #\r\n");
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

    send_raw("JOIN #s1mple\r\n");

    if(connection < 0) {
        error(__FILE__, "Error calling conn()\n");
    }

    int bytes_read;
    int bufptr = 0;
    int bufsize = 0;
    char ch;
    char chunk[MAX_RECV_LEN];
    size_t size_recv = 0, total_size  = 0;
    int more_flag = 0;
    int error_flag = 0;

    while(running) {
        // Clear memory
        memset(chunk, 0, MAX_RECV_LEN);

        // Receive data of length MAX_RECV_LEN to chunk
        if((size_recv = recv(sock, chunk, MAX_RECV_LEN, 0)) > 0) {
            printf("Bytes Received: %ld\n", size_recv);
            
            if(size_recv == -1) {
                error_flag = 1;
            } else if(size_recv == MAX_RECV_LEN) {
                printf(" - Need to store the next one\n");
                more_flag = 1;
            }

            for(int i = 0; i < size_recv; ++i) {
                printf("%d ", chunk[i]);
            }

            printf("\n");

            // chunk[size_recv] = '\0';
            // printf("Last char @ %ld is %c:%d\n", size_recv, chunk[size_recv], chunk[size_recv]);

            total_size += size_recv;
            printf("%s\n\n", chunk);
        }
        
        total_size = 0;
        // bytes_read = read(sock, &buffer, (size_t) MAX_RECV_LEN);

        // // Nothing is received, exit loop
        // if(bytes_read <= 0) break;

        // /*
        
        // Our current stream from the buffer states we need more data.
        // We will set a flag to indicate we are misssing data.
        // */
        // if(bytes_read == MAX_RECV_LEN) {
                        
        //     // This is the first time we are requesting for a longer buffer
        //     if(!more_flag) {
        //         more_flag = 1;
        //         strncpy(temp_buffer, buffer, sizeof(buffer));
                
        //     } else {

        //         // If we still receive more maxed buffers, we can handle them here
        //     }

        // } else {
        //     if(more_flag) {
        //         printf("Only one time bro\n");

        //         // Concate strings. Make sure we got EVERYTHING now
        //         strncat(temp_buffer, buffer, sizeof(buffer));
        //         printf("Temp Buffer: %s\n", temp_buffer);
        //         more_flag = 0;
        //         memset(temp_buffer, 0, sizeof(temp_buffer));
        //         continue;
        //     }
        // }

        // printf("Buffer: %s\n", buffer);
        
        // printf("Bytes Received: %d\n", bytes_read);
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