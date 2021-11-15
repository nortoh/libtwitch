#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "channel.h"

/* Currently no functions for user.c */

struct user_t* create_user(char* name, char* display_name) {
    struct user_t* user = malloc(sizeof(struct user_t));
    user->name = strdup(name);
    user->display_name = display_name;
    user->next = 0;
    return user;
}

void print_user(struct user_t* user) {
    printf("[User %p] Name: %s Next: %p\n", (void *) user, user->name, user->next);
}