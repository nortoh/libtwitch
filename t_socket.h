#include "irc_message.h"
#ifndef __T_SOCKET_H__
#define __T_SOCKET_H__

/**
 * @brief Connect to Twitch's chat server
 *
 * @return int
 */
extern int connect_to_twitch();

/**
 * @brief Thread body for starting connection
 *
 * @param vargs
 * @return void*
 */
void* thread_start(void *vargs);

/**
 * @brief Connect to chat server
 *
 * @param host
 * @param port
 * @return int
 */
extern int conn(char* host, int port);

/**
 * @brief Handle connection
 *
 * @param message
 */
void handle(struct irc_message_t irc_message);

/**
 * @brief Convert IRC string to IRC type
 *
 * @param raw
 * @return char*
 */
char* irc_2_type(char* raw);

/**
 * @brief Send raw string through socket
 *
 * @param raw
 * @return int
 */
int send_raw(char* raw);

/**
 * @brief Send channel message
 *
 * @param channel
 * @param message
 */
extern void send_channel_message(struct channel_t* channel, char message[500]);

/**
 * @brief Receive full application message
 *
 * @param more
 */
void receive_full_chunk(int* more);

/**
 * @brief Join channel
 *
 * @param channel
 */
extern void join_channel(char* channel);

/**
 * @brief Part channel
 *
 * @param name
 */
void part_channel(char* name);

/**
 * @brief Handle PRIVMSG
 *
 * @param raw
 */
void handle_privmsg(char* raw);

/**
 * @brief Handle CAP
 *
 */
void handle_cap();

/**
 * @brief Handle PART
 *
 * @param raw
 */
void handle_part(char* raw);

/**
 * @brief Handle USERNOTICE
 *
 * @param raw
 */
void handle_usernotice(char* raw);

/**
 * @brief Handle CLEARCHAT
 *
 * @param raw
 */
void handle_clearchat(char* raw);

/**
 * @brief Handle CLEARMSG
 *
 * @param raw
 */
void handle_clearmsg(char* raw);

/**
 * @brief Handle PING
 *
 */
void handle_ping(char* raw);

/**
 * @brief Handle HOSTTARGET
 *
 * @param raw
 */
void handle_hosttarget(char* raw);

/**
 * @brief Check if ID is location in LINE
 *
 * @param line
 * @param id
 * @return int
 */
extern int received_id(char* line, int code);
#endif
