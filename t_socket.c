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
#include "irc_message.h"
#include "user.h"
#include "message.h"
#include "channel.h"
#include "tag.h"

#define MAX_RECV_LEN 512 // 512B
#define FULL_BUFFER_MULTIPLE 2 * 50 // 50KB

static int sock = 0, running = 0, connected = 0, motd = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0, full_buffer_size = 0;
static char recv_buffer[MAX_RECV_LEN] = {0};
static char full_buffer[FULL_BUFFER_MULTIPLE * MAX_RECV_LEN];

int send_raw(char* raw) {
    if(!sock) return -1;
    printf("%s>%s Sending %s", GRN, RESET, raw);
    return send(sock, raw, strlen(raw), 0);
}

char* irc_2_type(char* raw) {
    // We must make sure PRIVMSG is not part of any other type
    // since it allows for "code injection".
    if(!strstr(raw, "PRIVMSG") == 0) return "PRIVMSG";
    if(!strstr(raw, "JOIN") == 0 && strstr(raw, "PRIVMSG") == 0) return "JOIN";
    if(!strstr(raw, "PART") == 0 && strstr(raw, "PRIVMSG") == 0) return "PART";
    if(!strstr(raw, "CAP") == 0 && strstr(raw, "PRIVMSG") == 0) return "CAP";
    if(!strstr(raw, "USERNOTICE") == 0 && strstr(raw, "PRIVMSG") == 0) return "USERNOTICE";
    if(!strstr(raw, "ROOMSTATE") == 0 && strstr(raw, "PRIVMSG") == 0) return "ROOMSTATE";
    if(!strstr(raw, "USERSTATE") == 0 && strstr(raw, "PRIVMSG") == 0) return "USERSTATE";
    if(!strstr(raw, "CLEARCHAT") == 0 && strstr(raw, "PRIVMSG") == 0) return "CLEARCHAT";
    if(!strstr(raw, "CLEARMSG") == 0 && strstr(raw, "PRIVMSG") == 0) return "CLEARMSG";
    if(!strstr(raw, "HOSTTARGET") == 0 && strstr(raw, "PRIVMSG") == 0) return "HOSTTARGET";
    if(!strstr(raw, "CTCP") == 0 && strstr(raw, "PRIVMSG") == 0) return "CTCP";
    if(!strstr(raw, "RECONNECT") == 0 && strstr(raw, "PRIVMSG") == 0) return "RECONNECT";
    if(!strstr(raw, "NOTICE") == 0 && strstr(raw, "PRIVMSG") == 0) return "NOTICE";
    return "NA";
}

void join_channel(char* channel) {
    char buffer[1024];
    sprintf(buffer, "JOIN %s\r\n", channel);
    add_channel(channel);
    send_raw(buffer);
}

void part_channel(char* name) {
    char buffer[1024];
    struct channel_t* channel = get_channel(name);
    sprintf(buffer, "PART %s\r\n", name);
    destroy_channel(channel);
    send_raw(buffer);
}

void send_channel_message(struct channel_t* channel, char message[500]) {
    char buffer[1024];
    sprintf(buffer, "PRIVMSG %s : %s\t\n", channel->name, message);
    send_raw(buffer);
}

int conn(char* host, int port) {

    // Initalize socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    printf("Created socket\n");
    
    // Convert IP to binary form
    if(inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        printf("Failed to convert IP to binary form\n");
        return -1;
    }

    // Connect socket to host
    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected!\n");

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
    free(copy);
    return received_id == id;
}

char* get_username_from_host(char* line) {
    char* username_result;
    char* username = strtok_r(line, "!", &username_result);
    memmove(username, username + 1, strlen(username));
    return username;
}

void handle_cap() {
    // printf("Received good ACK\n");
}

void handle_join(char* raw) {
    char* token;
    char* result;

    char username_str[30];
    char channel_str[30];

    int count = 0;
    for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        switch(count) {

            // Username and Host
            case 0:
                sprintf(username_str, "%s", get_username_from_host(token));
                break;
            
            // Do nothing
            case 1:
            
                break;
            
            // Channel
            case 2:
                sprintf(channel_str, "%s", token);
                break;
        }
        count++;
    }

    if(!strcmp(username_str, get_config_value("username"))) return;

    struct channel_t* channel = get_channel(channel_str);
    struct user_t* user = create_user(channel, username_str);
    printf("%sUser %s has joined%s %s\n", BLU, user->name, RESET, user->channel->name);
    free(user);
}

void handle_part(char* raw) {
    char* token;
    char* result;

    char username_str[30];
    char channel_str[30];

    int count = 0;
    for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        switch(count) {

            // Username and Host
            case 0:
                sprintf(username_str, "%s", get_username_from_host(token));
                break;
            
            // Do nothing
            case 1:
                break;
            
            // Channel
            case 2:
                sprintf(channel_str, "%s", token);
                break;
        }
        count++;
    }

    if(!strcmp(username_str, get_config_value("username"))) return;

    struct channel_t* channel = get_channel(channel_str);
    struct user_t* user = create_user(channel, username_str);
    printf("User %s has parted from %s\n", user->name, user->channel->name);
    free(user);
}

void handle_privmsg(char* raw) {
    int building = 0;
    char message_buffer[3 * 1024] = {0}; // 3KB
    size_t message_buffer_size = 0;

    char tag_str[500];
    char username_str[30];
    char channel_str[30];

    char* token;
    char* result;
    int count = 0;    
    
    struct tag_header_t* tag_header = 0;

    // Grab information for private message
    for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        switch(count) {
            // Tag
            case 0:
                sprintf(tag_str, "%s", token);
                // tag_header = create_tag(tag_str);
                break;

            // Sender
            case 1:
                sprintf(username_str, "%s", get_username_from_host(token));
                break;

            // Do nothing (irc_type)
            case 2:
                break;

            // Channel
            case 3:
                sprintf(channel_str, "%s", token);
                break;

            default:
                if(!building) {
                    building = 1;

                    // Create a copy and add a space
                    char token_copy[strlen(token) + 1];     // + 1 for additional space
                    strcpy(token_copy, token);
                    strcat(token_copy, " ");

                    // Copy over first word into message buffer
                    memcpy(message_buffer, token_copy, sizeof(token_copy));
                    message_buffer_size += sizeof(token_copy);
                } else {
                    // Create a copy and add a space
                    char token_copy[strlen(token) + 1];
                    strcpy(token_copy, token);
                    strcat(token_copy, " ");

                    // Copy over any additional words into message buffer
                    memcpy(message_buffer + message_buffer_size, token_copy, sizeof(token_copy));
                    message_buffer_size += sizeof(token_copy);
                }
                break;
        }
        count++;    
    }
    
    memmove(message_buffer, message_buffer + 1, strlen(message_buffer));    // Remove ":"
    message_buffer[message_buffer_size - 1] = '\0';

    struct channel_t* channel = get_channel(channel_str);
    struct user_t* sender = create_user(channel, username_str);
    
    // If we do not free these users eventually, we will fill up our heap!!!
    struct message_t message_block = create_message(channel, sender, message_buffer);
    print_message_block(&message_block);

    // printf("The tag:\n");
    // print_tag_header(tag_header);
    
    // destroy_tag_header(tag_header);
    // Clear buffers
    memset(message_buffer, 0, sizeof(message_buffer));
    free(sender);
}

void handle_usernotice(char* raw) {
    printf("Usernotice: %s\n", raw);
}

void handle_clearchat(char* raw) {
    char tag_str[500];
    char username_str[30];
    char channel_str[30];

    char* token;
    char* result;
    int count = 0;    

    // Grab information for private message
    for(token = strtok_r(raw, " ", &result); token != 0; token = strtok_r(0, " ", &result)) {
        switch(count) {
            // Tag
            case 0:
                sprintf(tag_str, "%s", token);
                // struct tag_header_t* tag = create_tag(tag_str);
                break;

            // Sender
            case 1:
                break;

            // Do nothing (irc_type)
            case 2:
                break;

            // Channel
            case 3:
                sprintf(channel_str, "%s", token);
                break;
            
            case 4:
                sprintf(username_str, "%s", token);
                memmove(username_str, username_str + 1, strlen(username_str));
                break;
        }
        count++;
    }

    printf("CLEARCHAT [%s:%s]\n", channel_str, username_str);

}

void handle_clearmsg(char* raw) {
    printf("Clearmsg: %s\n", raw);
}

void handle_ping() {
    mark(20);
    send_raw("PONG :tmi.twitch.tv\r\n");
}

void handle(char* raw) {
    // Checks
    if(!connected) { 
        connected = received_id(raw, 1);
        join_channel("#xqcow");
        join_channel("#illojuan");
        join_channel("#elweroking");
        join_channel("#casimito");
        join_channel("#loltyler1");
    }

    if(!motd) {
        motd = received_id(raw, 376);
        return;
    } 

    char* irc_type = irc_2_type(raw);
    // printf("Received: %s\n", irc_type);
    // printf("< %s\n", raw);

    if(!strcmp("PRIVMSG", irc_type)) {
        handle_privmsg(raw);
    } else if(!strcmp("JOIN", irc_type)) {
        handle_join(raw);
        // printf("< %s\n\n\n", raw);
    } else if(!strcmp("PART", irc_type)) {
        handle_part(raw);
    } else if(!strcmp("PING", irc_type)) {
        handle_ping();
    } else if(!strcmp("CAP", irc_type)) {
        handle_cap();
    } else if(!strcmp("USERNOTICE", irc_type)) {
        handle_usernotice(raw);
    } else if(!strcmp("NOTICE", irc_type)) {
        // handle_privmsg(raw);
    } else if(!strcmp("CLEARCHAT", irc_type)) {
        handle_clearchat(raw);
    } else if(!strcmp("CLEARMSG", irc_type)) {
        handle_clearmsg(raw);
    } else if(!strcmp("HOSTTARGET", irc_type)) {
        // handle_privmsg(raw);
    } else if(!strcmp("CTCP", irc_type)) {
        // handle_privmsg(raw);
    } else if(!strcmp("RECONNECT", irc_type)) {
        // handle_privmsg(raw);
    }

}

void receive_full_chunk(int* more_flag) {
    
    // Clear the recv_buffer before reading in more
    memset(recv_buffer, 0, sizeof(recv_buffer));

    if((bytes_recv = recv(sock, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
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
        printf("Error calling conn()\n");
    }

    int more_flag = 0;

    // Clear recv_buffer
    memset(recv_buffer, 0, sizeof(recv_buffer));

    sleep(1);

    while(running) {

        // Read in a stream of up to 512B
        if((bytes_recv = recv(sock, recv_buffer, sizeof(recv_buffer), 0)) > 0) {

            if(bytes_recv <= 0) {
                printf("Connection aborted\n");
                break;
            }

            // Check if our buffer ends with /r/n
            int newline_flag = recv_buffer[bytes_recv - 1] == '\n';
            int carriage_flag = recv_buffer[bytes_recv - 2] == '\r';

            // If it does not, dump the recv buffer into the full_buffer
            // and continue loading more data until /r/n
            if(!(newline_flag && carriage_flag)) {
                memmove(full_buffer, recv_buffer, bytes_recv * sizeof(char));
                full_buffer_size += bytes_recv;
                more_flag = 1;

                while(more_flag) {
                    receive_full_chunk(&more_flag);
                }
            } else {
                // We have /r/n, copy the recv_buffer to the full_buffer
                memmove(full_buffer, recv_buffer, bytes_recv * sizeof(char));
            }

            // Add null termination 
            full_buffer[full_buffer_size - 1] = '\0';
            full_buffer_size = 0;

            char* token;
            char* result;
            for(token = strtok_r(full_buffer, "\r\n", &result); token != 0; token = strtok_r(0, "\r\n", &result)) {
                handle(token);
            }
            
            // Clear memory when we are done
            memset(full_buffer, 0, sizeof(full_buffer));
            memset(recv_buffer, 0, sizeof(recv_buffer));
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

    printf("Creating socket thread\n");
    if(pthread_create(&socket_thread, 0, thread_start, 0) != 0) {
        printf("Could not create socket thread!!!\n");
        return 0;
    }

    pthread_join(socket_thread, 0);

    return 1;
}