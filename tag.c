#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tag.h"
#include "badge.h"

static struct tag_t* head = 0;

void print_tag_header(struct tag_header_t* header) {
    struct tag_t* curr = header->tags;

    while(curr) {
        printf("[%p:%p] %s = %s\n", (void *) curr, (void *) curr->next, curr->key, curr->value);
        curr = curr->next;
    }
}

void free_tag_header(struct tag_header_t* header) {
    struct tag_t* curr = header->tags;
    
    // if(header->badges) free_badges(header->badges);

    while(curr) {
        struct tag_t* tmp = curr;
        curr = curr->next;
        free(tmp);
        printf("Freeing tag %p\n", (void *) tmp);
    }
}

struct tag_header_t* create_tag(char* raw) {
    struct tag_header_t* header = malloc(sizeof(struct tag_header_t));
    disassemble_tag(header, raw);
    return header;
}

void destroy_tag_header(struct tag_header_t* header) {
    if(!header) return;

    struct tag_t* curr = header->tags;

    while(curr) {
        struct tag_t* tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    free(header);
}

void disassemble_tag(struct tag_header_t* header, char* raw) {
    char tag_str[1024];
    strcpy(tag_str, raw); 
    memmove(tag_str, tag_str + 1, sizeof(tag_str));

    char* token_split;
    char* result_split;

    char* token_pair;
    char* token_result;

    char key[50];
    char value[50];
    struct tag_t* head = 0;
    struct tag_t* curr = 0;

    printf("======================\n");
    for(token_split = strtok_r(tag_str, ";", &result_split); token_split != 0; token_split = strtok_r(0, ";", &result_split)) {
        int pair_count = 0;
        for(token_pair = strtok_r(token_split, "=", &token_result); token_pair != 0; token_pair = strtok_r(0, "=", &token_result)) {
            if(pair_count == 0) {
                strcpy(key, token_pair);
            } else {
                strcpy(value, token_pair);
            }
            pair_count++;
        }
        
        // Empty value
        if(pair_count == 1) {
            strcpy(value, "0");
        }

        struct tag_t* next = create_tag_pair(key, value);
        
        if(!strcmp(key, "badges") && strcmp(value, "0")) {
            struct badge_t* badges = disassemble_badge(value);
            print_badges(badges);
            header->badges = badges;
        }
        
        if(!head) {
            head = next;
            curr = next;
        } else {
            curr->next = next;
            curr = curr->next;
        }
    }
    header->tags = head;
    print_tag_header(header);
    free_tag_header(header);
    printf("======================\n");
}

struct tag_t* create_tag_pair(char* key, char* value) {
    struct tag_t* tag = malloc(sizeof(struct tag_t));
    strcpy(tag->key, key);
    strcpy(tag->value, value);
    tag->next = 0;
    tag->badges = 0;
    return tag;

}

void add_tag_node(struct tag_t* node) {
    node->next = head;
    head = node;
}

char* get_tag_value(struct tag_t* tag, char* key) {
    
}