#ifndef __IRC_MESSAGE_H__
#define __IRC_MESSAGE_H__
struct irc_message_t {
    char* data;
};

extern struct irc_message_t create_irc_message(char* line);
#endif