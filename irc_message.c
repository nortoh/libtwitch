#include <stdlib.h>
#include <string.h>
#include "irc_message.h"

/**
 * @brief Create a IRC message object
 *
 * @param data
 * @return struct irc_message_t
 */
struct irc_message_t create_irc_message(char* data) {
    struct irc_message_t message;
    message.data = data;
    message.len = strlen(data);
    return message;
}
