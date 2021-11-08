#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

struct channel_t {
    char* name;
    char* chan_name;
    struct user_t* users;
    struct channel_t* next;
};

static struct channel_t *head = 0;
static int size = 0;

struct channel_t* create_channel(char* name) {
    struct channel_t* channel = malloc(sizeof(struct channel_t));
    channel->chan_name = malloc((strlen(name) + 1) * sizeof(char));
    sprintf(channel->chan_name, "#%s", name);
    channel->name = name;
    channel->users = 0;
    channel->next = 0;
    
    return channel;
}

void list_channels() {
    struct channel_t* curr = head;

    while(curr) {
        printf("[Channel %p] name=%s, next=%p\n", curr, curr->name, curr->next);

        curr = curr->next;
    }
}

int has_channel(struct channel_t* channel) {
    struct channel_t* curr = head;

    while(curr) {    
        if(curr == channel) return 1;
        curr = curr->next;
    }

    return 0;
}

void add_channel(struct channel_t* channel) {
    if(has_channel(channel)) return;

    if(!head) {
        head = channel;
        return;
    }

    // Add to the top of the list
    channel->next = head;
    head = channel;

    size++;
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

void remove_users(struct channel_t* channel) {
    struct user_t* curr = channel->users;

    while(curr) {
        curr = curr->next;
    }
}

int destroy_channel(struct channel_t* channel) {
    if(!has_channel(channel)) return 0;

    free(channel->chan_name);
    remove_users(channel);
    free(channel);

    return 1;
}

void add_user(struct channel_t* channel, struct user_t* user) {
    if(!channel->users) {
        channel->users = user;
        printf("[Channel] Added %s to %s\n", user->name, channel->chan_name);
        return;
    }

    user->next = channel->users;
    channel->users = user;

    printf("[Channel] Added %s to %s\n", user->name, channel->chan_name);
}

int has_user(struct channel_t* channel, char* username) {
    struct user_t* curr = channel->users;

    while(curr) {
        if(curr->name == username) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}