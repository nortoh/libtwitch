#ifndef __CHANNEL_H__
#define __CHANNEL_H__
struct channel_t {
    char* name;
    char* chan_name;
    struct user_t* users;
    struct channel_t* next;
};
struct channel_t* create_channel(char* name);
extern void list_channels();
extern struct channel_t* get_channel(char* name);
extern struct channel_t* add_channel(char* name);
extern void remove_channel(char* name);
extern int destroy_channel(char* name);
void remove_users(struct channel_t* channel);
extern int has_channel(char* name);
extern void list_users(struct channel_t* channel);
extern struct user_t* get_user(struct channel_t* channel, char* username);
extern struct user_t* add_user(struct channel_t* channel, char* username);
extern void remove_user(struct channel_t* channel, struct user_t* user);
extern int has_user(char* channel_name, char* username);
#endif