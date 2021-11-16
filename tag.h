
#ifndef __TAG_H__

struct tag_header_t {
    // char id[40];
    struct tag_t* tags;
    struct badge_t* badges;
};

struct tag_t {
    char key[50];
    char value[50];
    char* target; // channel or user
    struct badge_t* badges;
    struct tag_t* next;
};

extern struct tag_header_t* create_tag(char* raw);
void print_tag_header(struct tag_header_t* header);
struct tag_t* create_tag_pair(char* key, char* value);
extern void free_tag_header(struct tag_header_t* header);
extern void disassemble_tag(struct tag_header_t* header, char* raw);
extern void add_tag_node(struct tag_t* node);
extern char* get_tag_value(struct tag_t* tag, char* key);
#define __TAG_H__
#endif