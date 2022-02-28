#include <stdio.h>
#include <sys/time.h>
#include "user.h"
#include "channel.h"
#include "tag.h"
#include "utils.h"
#include "message.h"

/**
 * @brief Create a message object
 *
 * @param channel
 * @param sender
 * @param msg
 * @return struct message_t
 */
struct message_t create_message(struct channel_t* channel, struct user_t* sender, char* msg) {
    struct message_t message;
    message.channel = channel;
    message.sender = sender;
    gettimeofday(&message.time, 0);
    message.message = msg;
    return message;
}

/**
 * @brief Print message block
 *
 * @param message_block
 */
void print_message_block(struct message_t* message_block) {
    printf(RESET "[" GRN "%s" RESET "/" GRN "%s" RESET "] - " CYN "%s\n", message_block->sender->name, message_block->channel->name, message_block->message);
}
