#include <stdlib.h>
#include <string.h>

struct irc_message_t {
    char* data;
    int len;
};

struct irc_message_t create_irc_message(char* data) {
    struct irc_message_t message;
    message.data = data;
    message.len = strlen(data);
    return message;
}