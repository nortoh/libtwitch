#ifndef __USER_H__
struct user_t {
    char* name;
    char* display_name;
    struct user_t* next;
};

extern struct user_t* create_user(char* user, char* display_name);
extern void print_user(struct user_t* user);
 
#define __USER_H__
#endif