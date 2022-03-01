#include <sys/time.h>
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/**
 * @brief Message structure
 *
 */
struct message_t {
    char* message;
    struct user_t* sender;
    struct channel_t* channel;
    struct tag_header_t* tag;
    struct timeval time;
};

/**
 * @brief Create a message object
 *
 * @param channel
 * @param sender
 * @param msg
 * @return struct message_t
 */
extern struct message_t create_message(struct channel_t* channel, struct user_t* sender, char* msg);

/**
 * @brief Print message block
 *
 * @param message_block
 */
extern void print_message_block(struct message_t* message_block);
#endif
