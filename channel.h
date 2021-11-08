struct channel_t {
    char* name;
    char* chan_name;
    struct user_t* users;
    struct channel_t* next;
};

extern struct channel_t* create_channel(char* name);

extern void list_channels();

extern struct channel_t *get_channel(char* name);

extern void add_channel(struct channel_t* channel);

extern int has_channel(struct channel_t* channel);

extern void add_user(struct channel_t* channel, struct user_t* user);

extern int has_user(struct channel_t* channel, char* username);