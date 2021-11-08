#include <stdio.h>
#include "user.h"

struct channel_t {
    char *name;
    struct channel_t *next;
    struct user_t* users;
};

static struct channel_t *head = 0;

void list_channels() {
    struct channel_t* curr = head;

    while(curr) {
        printf("[Channel %p] name=%s, next=%p\n", curr, curr->name, curr->next);

        curr = curr->next;
    }
}

void add_channel(struct channel_t* channel) {
    if(!head) {
        head = channel;
        return;
    }

    // Add to the top of the list
    channel->next = head;
    head = channel;
}

struct channel_t* get_channel(char *name) {
    struct channel_t* curr = head;

    while(curr) {
        if(curr->name == name) {
            return curr;
        }
        curr = curr->next;
    }
    return 0;
}

int has_channel(struct channel_t* channel) {
    struct channel_t* curr = head;

    while(curr) {    
        if(curr == channel) return 1;
        curr = curr->next;
    }

    return 0;
}