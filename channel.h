#ifndef __CHANNEL_H__
#define __CHANNEL_H__
struct channel_t {
    char name[30];
    struct channel_t* next;
};
struct channel_t* create_channel(char* name);
extern void list_channels();
extern struct channel_t* get_channel(char* name);
extern struct channel_t* add_channel(char* name);
extern void remove_channel(struct channel_t* channel);
extern int destroy_channel(struct channel_t* channel);
extern int has_channel(struct channel_t* channel);
extern void destroy_channels();
#endif