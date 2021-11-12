#include "user.h"
#include "tag.h"

struct message_t {
    char* message;
    struct user_t* sender;
    struct channel_t* channel;
    struct tag_t* tag;
};

extern struct message_t create_message(char* msg, char* username, char* channel_name);