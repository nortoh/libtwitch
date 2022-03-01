#ifndef __USER_H__
#define __USER_H__

/**
 * @brief User structure
 *
 */
struct user_t {
    char name[30];
    struct channel_t* channel;
};

/**
 * @brief Create a user object
 *
 * @param channel
 * @param name
 * @return struct user_t*
 */
extern struct user_t* create_user(struct channel_t* channel, char* name);

/**
 * @brief Print user message
 *
 * @param user
 */
extern void print_user(struct user_t* user);
#endif
