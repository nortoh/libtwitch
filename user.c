#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "channel.h"

struct user_t* create_user(struct channel_t* channel, char* name) {
    struct user_t* user = malloc(sizeof(struct user_t));
    sprintf(user->name, "%s", name);
    user->channel = channel;
    return user;
}

void print_user(struct user_t* user) {
    printf("[User %p] Name: %s\n", (void *) user, user->name);
}
