#ifndef __USER_H__
#define __USER_H__
struct user_t {
    char name[30];
    struct channel_t* channel;
};
extern struct user_t* create_user(struct channel_t* channel, char* name);
extern void print_user(struct user_t* user);
#endif