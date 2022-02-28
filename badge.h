#ifndef __BADGE_H__
#define __BADGE_H__
struct badge_t {
    char key[50];
    char value[50];
    struct badge_t* next;
};

extern void print_badges(struct badge_t* badge);
extern void free_badges(struct badge_t* badge);
extern struct badge_t* disassemble_badge(char* raw);
struct badge_t* create_badge_pair(char* key, char* value);
#endif
