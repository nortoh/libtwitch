#ifndef __T_SOCKET_H__
extern int connect_to_twitch();

void handle(char* raw);
char* irc_2_type(char* raw);
int send_raw(char* raw);
void receive_full_chunk(int* more_flag);
extern void join_channel(char* channel);
void handle_privmsg(char* raw);
void handle_ping(char* raw);
extern int conn(char* host, int port);
extern int received_id(char* line, int code);
#define __T_SOCKET_H__
#endif