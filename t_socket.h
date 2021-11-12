extern int connect_to_twitch();

void handle(char* raw);
char* irc_2_type(char* raw);
int send_raw(char* raw);
int recv_raw(int fd, char* buffer, size_t size);

extern int conn(char* host, int port);
extern int received_id(char* line, int code);