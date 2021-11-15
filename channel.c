#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

struct channel_t {
    char* name;
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
        printf("Looking for channel %s\n", name);
        if(strcmp(curr->name, name) == 0) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

struct channel_t* create_channel(char* name) {
    struct channel_t* channel = malloc(sizeof(struct channel_t));
    asprintf(&channel->name, "%s", name);
    channel->users = 0;
    channel->next = 0;
    
    return channel;
}

struct channel_t* add_channel(char* name) {
    struct channel_t* channel = create_channel(name);

    channel->next = head;
    head = channel;
    size++;

    return channel;
}

void remove_channel(char* name) {

    // If we are removing the top channel
    if(!strcmp(head->name, name)) {
        struct channel_t* tmp = head;
        head = head->next;
        free(tmp);
        return;
    }

    struct channel_t* curr = head;
    struct channel_t* prev = head;

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
    struct channel_t* curr = head;

    while(curr) {
        if(strcmp(curr->name, name) == 0) {
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
    remove_channel(name);
    free(channel->name);
    free(channel);

    return 1;
}

void list_users(struct channel_t* channel) {
    struct user_t* curr = channel->users;

    int cycle = 0;
    while(curr) {
        print_user(curr);
        curr = curr->next;
        cycle++;
    }

}

int has_user(char* channel_name, char* username) {
    struct channel_t* channel = get_channel(channel_name);
    struct user_t* curr = channel->users;

    while(curr) {
        if(strcmp(curr->name, username) == 0) { 
            return 1; 
        } 
        curr = curr->next;
    }

    return 0;
}

void remove_user(struct channel_t* channel, struct user_t* user) {
    struct user_t* curr = channel->users;

    if(!strcmp(curr->name, user->name)) {
        struct user_t* tmp = curr;
        head = head->next;
        free(tmp);
        return;
    }
    
    struct user_t* prev = 0;
    while(curr) {

        if(!strcmp(curr->name, user->name)) {
            prev->next = curr->next;
            free(curr);
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

struct user_t* get_user(struct channel_t* channel, char* username) {;
    struct user_t* user = channel->users;
    if(!user) return 0;

    int cycles = 0;
    while(user) {
        int found = strcmp(user->name, username) == 0;        
        if(found) return user;
        cycles++;
        user = user->next;
    }

    return 0;
}

struct user_t* add_user(struct channel_t* channel, char* username) {
    struct user_t* user = create_user(username, username);

    user->next = channel->users;
    channel->users = user;
    return user;
}