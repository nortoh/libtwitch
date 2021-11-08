struct channel_t {
    char *name;
    struct channel_t *next;
};

extern void add_channel(struct channel_t *channel);

extern struct channel_t *get_channel(char *name);