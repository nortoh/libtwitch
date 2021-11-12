#define _GNU_SOURCE

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


void list_channels() {
    struct channel_t* curr = head;

    while(curr) {
        printf("[Channel %p] name=%s, next=%p\n", curr, curr->name, curr->next);

        curr = curr->next;
    }
}

int has_channel(char* name) {
    struct channel_t* curr = head;

    while(curr) {    
        if(!strcmp(curr->name, name)) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

struct channel_t* create_channel(char* name) {
    if(has_channel(name)) return 0;

    struct channel_t* channel = malloc(sizeof(struct channel_t));
    asprintf(&channel->chan_name, "%s", name);
    channel->name = name;
    channel->users = 0;
    channel->next = 0;
    
    return channel;
}

void add_channel(char* name) {
    if(has_channel(name)) return;

    struct channel_t* channel = create_channel(name);

    // Add to the top of the list
    channel->next = head;
    head = channel;

    size++;
}

void remove_channel(char* name) {
    if(!has_channel(name)) return;

    // If we are removing the top channel
    if(!strcmp(head->name, name)) {
        struct channel_t* tmp = head;
        head = head->next;
        free(tmp);
        return;
    }

    struct channel_t* curr = head;
    struct channel_t* prev = 0;

    while(curr) {
        if(!strcmp(curr->name, name)) {
            prev->next = curr->next;
            free(curr);
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

struct channel_t* get_channel(char *name) {
    if(!has_channel(name)) return 0;

    struct channel_t* curr = head;

    while(curr) {
        if(!strcmp(curr->name, name)) {
            return curr;
        }
        curr = curr->next;
    }

    return 0;
}

void remove_users(char* name) {
    struct channel_t* channel = get_channel(name);

    // Channel does not exist
    if(!channel) return;

    struct user_t* curr = channel->users;

    while(curr) {
        struct user_t* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}

int destroy_channel(char* name) {

    // Channel does not exist
    if(!has_channel(name)) return 0;

    struct channel_t* channel = get_channel(name);

    remove_users(name);
    free(channel->chan_name);
    free(channel);

    return 1;
}

void list_users(char* name) {
    struct channel_t* channel = get_channel(name);
    if(!channel) return;

    struct user_t* user = channel->users;

    while(user) {
        printf("[%s] %s\n", name, user->name);
        user = user->next;
    }

}

int has_user(char* channel_name, char* username) {
    struct channel_t* channel = get_channel(channel_name);
    struct user_t* curr = channel->users;

    while(curr) {
        if(!strcmp(curr->name, username)) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

struct user_t* get_user(char* channel_name, char* username) {
    if(!has_user(channel_name, username)) return 0;

    struct channel_t* channel = get_channel(channel_name);
    struct user_t* user = channel->users;

    while(user) {
        if(!strcmp(user->name, username)) return user;
        user = channel->users->next;
    }
}

void add_user(char* channel_name, char* username) {
    struct channel_t* channel = get_channel(channel_name);
    struct user_t* user = create_user(username, username);

    if(!channel->users) {
        channel->users = user;
        printf("[Channel] Added %s to %s\n", user->name, channel->chan_name);
        return;
    }

    user->next = channel->users;
    channel->users = user;

    printf("[Channel] Added %s to %s\n", user->name, channel->chan_name);
}