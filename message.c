#include <stdio.h>
#include <sys/time.h>
#include "user.h"
#include "channel.h"

struct message_t {
    char* message;
    struct user_t* sender;
    struct channel_t* channel;
    struct timeval time;
    struct tag_t* tag;
};

struct message_t create_message(struct channel_t* channel, struct user_t* sender, char* msg) {
    struct message_t message;
    message.channel = channel;
    message.sender = sender;
    gettimeofday(&message.time, 0);
    message.message = msg;
    message.tag = 0;
    return message;
}

void print_message_block(struct message_t* message_block) {
    printf("[%s/%s] - %s\n", message_block->sender->name, message_block->channel->name, message_block->message);
}