#ifndef __IRC_MESSAGE_H__
#define __IRC_MESSAGE_H__

/**
 * @brief IRC message structure
 *
 */
struct irc_message_t {
    char* data;
    int len;
};

/**
 * @brief Create a IRC message object
 *
 * @param data
 * @return struct irc_message_t
 */
extern struct irc_message_t create_irc_message(char* line);
#endif
