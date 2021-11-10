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

extern int has_channel(char* name);

extern void add_user(char* name, struct user_t* user);

extern int has_user(char* name, char* username);