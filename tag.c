#include <stdlib.h>
#include <stdio.h>

struct tag_t {
    char* key;
    char* value;
    char* target; // channel or user
    struct tag_t* next;
};

static struct tag_t* head = 0;

struct tag_t* create_tag(char* string) {
    struct tag_t tag;
    
    // tag->

    return &tag;
}


void add_tag_node(struct tag_t* node) {
    node->next = head;
    head = node;
}

char* get_tag_value(struct tag_t* tag, char* key) {
    
}