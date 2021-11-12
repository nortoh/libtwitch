struct channel_t {
    char* name;
    char* chan_name;
    struct user_t* users;
    struct channel_t* next;
};

struct channel_t* create_channel(char* name);

extern void list_channels();

extern struct channel_t *get_channel(char* name);

extern void add_channel(char* name);

extern void remove_channel(char* name);

void remove_users(char* name);

extern int has_channel(char* name);

extern void list_users(char* name);

extern struct user_t* get_user(char* channel_name, char* username);

extern void add_user(char* channel_name, char* username);

extern int has_user(char* channel_name, char* username);