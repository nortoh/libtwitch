struct channel_t {
    char* name;
    struct channel_t* next;
};

extern void list_channels();

extern struct channel_t *get_channel(char* name);

extern void add_channel(struct channel_t* channel);

extern int has_channel(struct channel_t* channel);