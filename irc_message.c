#include <stdlib.h>

struct irc_message_t {
    char* data;
};

struct irc_message_t create_irc_message(char* data) {
    struct irc_message_t message;
    message.data = data;
    return message;
}