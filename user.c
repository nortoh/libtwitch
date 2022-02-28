#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "channel.h"

/**
 * @brief Create a user object
 *
 * @param channel
 * @param name
 * @return struct user_t*
 */
struct user_t* create_user(struct channel_t* channel, char* name) {
    struct user_t* user = malloc(sizeof(struct user_t));
    sprintf(user->name, "%s", name);
    user->channel = channel;
    return user;
}

/**
 * @brief Print user message
 *
 * @param user
 */
void print_user(struct user_t* user) {
    printf("[User %p] Name: %s\n", (void *) user, user->name);
}
