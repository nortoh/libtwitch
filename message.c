#include "user.h"
#include "channel.h"

struct message_t {
    char* message;
    struct user_t* sender;
    struct channel_t* channel;
    struct tag_t* tag;
};

struct message_t create_message(char* msg, char* username, char* channel_name) {
    struct channel_t* channel = get_channel(channel_name);
    // struct user_t* sender = 
    struct message_t message;

    message.message = msg;



}