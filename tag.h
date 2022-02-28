#ifndef __TAG_H__
#define __TAG_H__
struct tag_header_t {
    // char id[40];
    struct tag_t* tags;
    struct badge_t* badges;
};

struct tag_t {
    char key[128];
    char value[128];
    struct badge_t* badges;
    struct tag_t* next;
};

extern struct tag_header_t* create_tag(char* raw);
void print_tag_header(struct tag_header_t* header);
struct tag_t* create_tag_pair(char* key, char* value);
extern void free_tag_header(struct tag_header_t* header);
extern void disassemble_tag(struct tag_header_t* header, char* raw);
extern void destroy_tag_header(struct tag_header_t* header);
#endif
