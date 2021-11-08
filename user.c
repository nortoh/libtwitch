#include "user.h"
#include "channel.h"
#include <stdlib.h>

/* Currently no functions for user.c */

struct user_t* create_user(char* name, char* display_name) {
    struct user_t* user = malloc(sizeof(struct user_t));
    user->name = name;
    user->display_name = display_name;
    user->next = 0;
    return user;
}
