#ifndef __MESSAGE_H__
#define __MESSAGE_H__
struct message_t {
    char* message;
    struct user_t* sender;
    struct channel_t* channel;
    struct tag_header_t* tag;
};

extern struct message_t create_message(struct channel_t* channel, struct user_t* sender, char* msg);
extern void print_message_block(struct message_t* message_block);
#endif