#include <stdio.h>

struct channel_t {
    char *name;
    struct channel_t *next;
};

static struct channel_t *head = 0;

void add_channel(struct channel_t *channel) {
    if(!head) head = channel;

    // Add to the top of the list
    channel->next = head;
    head = channel;

    printf("Adding new channel\n", channel->name);
}