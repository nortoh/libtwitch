struct channel_t {
    char *name;
    struct channel_t *next;
};

extern void add_channel(struct channel_t *channel);